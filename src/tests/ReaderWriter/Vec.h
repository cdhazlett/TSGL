/* Vec.h provides a vector class template named Vec, as needed for the Reader-Writer visualization.
 * Vec structure adapted from coursework in Professor Joel Adams' CS 112 class.
 * Reader and writer preference locks logic adapted from: http://lass.cs.umass.edu/~shenoy/courses/fall08/lectures/Lec11.pdf
 * Monitor locks logic adapted from: http://www.cse.psu.edu/~trj1/cse473-s08/slides/cse473-lecture-12-synch.pdf
 */

#ifndef VEC_H_
#define VEC_H_

//#include <tsgl.h>
#include <pthread.h>
#include <unistd.h>
#include "../../TSGL/tsgl.h"
using namespace tsgl;

template<class Item>

/**
 * Vec class contains the data necessary in order to create a custom Vec object.
 * Used as shared data in the Reader-Writer visualization.
 */
class Vec {
public:
	enum LockMethod { readerPref, writerPref, monitors }; // enum for type of lock
	Vec(); // Default constructor
	Vec(unsigned size, unsigned max, LockMethod pref); // Explicit constructor
	unsigned getSize() const  { return mySize; }  // get current capacity of the Vec
	unsigned getItems() const { return myItems; } // get number of items that have been appended
	unsigned getMax() const   { return myMax; } // get maximum capacity
	void     setSize(unsigned newSize);
	Item&    operator[](unsigned i);
	Item&    operator[](const unsigned i) const;
	Item&    getItem(int i) { return myArray[i]; }
	void     write(Item it, unsigned index);
	Item     read();
	//Lock and Unlock methods
	void     beginRead();
	void     endRead();
	void     beginWrite();
	void     endWrite();
	virtual ~Vec(); //Destructor
	

private:
	// methods for readerPref
	void beginReadRP();
	void endReadRP();
	void beginWriteRP();
	void endWriteRP();
	
	// methods for writerPref
	void beginReadWP();
	void endReadWP();
	void beginWriteWP();
	void endWriteWP();
	
	// methods for monitors
	void beginReadM();
	void endReadM();
	void beginWriteM();
	void endWriteM();
	
	unsigned 	mySize;
	unsigned 	myItems;
	unsigned	myMax;
	Item *   	myArray;
	LockMethod 	myLockMethod;
	
	// fields for locking in readerPref
	int readcount;			// number of readers in Vec
	pthread_mutex_t mutex; 	// mutex to regulate readcount
	pthread_mutex_t wrt;	// mutex to regulate writing
	
	// fields for locking in writerPref
	int writers, readers;
	pthread_mutex_t write_mutex, read_mutex, write_block, read_block, write_pending;
	
	// fields for locking in monitors
	int numReaders, numWriters;
	pthread_mutex_t mut;
	pthread_cond_t writeOK, readOK;
	//pthread_rwlock_t rwlock;
	
	friend class VecTester;
};

/**
 * Default constructor for the Vec class.
 */
template<class Item>
Vec<Item>::Vec() {
	mySize = myItems = myMax = 0;
	myArray = NULL;
	myLockMethod = readerPref;
	readcount = readers = writers = 0;
}

/**
 * Explicit constructor for the Vec class.
 * @param size, the initial size of the Vec
 * @param locks, the type of locks to use
 */
template<class Item>
Vec<Item>::Vec(unsigned size, unsigned max, LockMethod locks) {
	myItems = 0;
	mySize = size;
	myMax = max;
	myArray = new Item[size];
	for(unsigned i = 0; i < size; i++ ) {
		myArray[i] = Item();
	}
	myLockMethod = locks;
	readcount = readers = writers = 0;
}

/**
 * setSize() changes the size of the Vec.
 * @param newSize, an unsigned for the new length of the Vec.
 * Postcondition: Vec has a capacity of newSize
 */
template <class Item>
void Vec<Item>::setSize(unsigned newSize) {
	if(mySize != newSize) {
		if(newSize==0) {
			delete [] myArray;
			myArray = NULL;
			mySize = 0;
		} else {
			Item* newArray = new Item[newSize];
			if(mySize < newSize) {
				for(unsigned i = 0; i < newSize; i++) {
					if(i < mySize) {
						newArray[i] = myArray[i];
					} else {
						newArray[i] = Item();
					}
				}
			} else {
				for(unsigned i = 0; i < newSize; i++) {
					newArray[i] = myArray[i];
				}
			}
			mySize = newSize;
			delete [] myArray;
			myArray = newArray;
		}
	}
}

/**
 * Assigns a value to an index in the vector
 * @param i, an unsigned
 * Return: the item at index i
 * Postcondition: myArray[i] is assigned to the specified item by =
 */
template <class Item>
Item& Vec<Item>::operator[](unsigned i) {
	if(i>=mySize) {
		throw std::range_error("index out of range");
	} else {
		return myArray[i];
	}
}

/** 
 * Returns the value at index i in our Vec
 * @param i, an unsigned
 * Return: a value at index i in the Vec
 */
template <class Item>
Item& Vec<Item>::operator[](unsigned i) const {
    if ( i >= mySize) {
        throw std::range_error("Index is out of range");
    }
    return myArray[i];
}

/**
 * write() adds an item at the next available location in our Vec
 * @param it, an Item to add at the next location
 */
template <class Item>
void Vec<Item>::write(Item it, unsigned index) {
	while (index >= mySize) {
		if( myMax < index )
			throw std::range_error("index beyond max");
		if( mySize*2 < myMax ) {
			setSize(mySize*2);
		} else {
			setSize( myMax );
		}
	}
	myArray[index] = it; // write it
	if (index == myItems) { // adding item at next available position
		myItems++;
	}
}

/**
 * read() gets a random item appended to our Vec
 * Return: an item in the Vec
 */
template <class Item>
Item Vec<Item>::read() {
	while (myItems == 0) {
		endRead();
		sleep(0.1);
		beginRead();
	}
	return myArray[rand()%myItems];		// read a random item
}

/**
 * beginRead() locks the appropriate locks for the calling thread to call read()
 * endRead() must be called later by the thread
 */
template <class Item>
void Vec<Item>::beginRead() {
	switch(myLockMethod) {
		case readerPref:
			beginReadRP();
			break;
		case writerPref:
			beginReadWP();
			break;
		case monitors:
			beginReadM();
			break;
		default:
			beginReadWP();
			break;
	}
}

/**
 * endRead() unlocks the appropriate locks when the calling thread has called read()
 * beginRead() must be called by the thread before endRead()
 */
template <class Item>
void Vec<Item>::endRead() {
	switch(myLockMethod) {
		case readerPref:
			endReadRP();
			break;
		case writerPref:
			endReadWP();
			break;
		case monitors:
			endReadM();
			break;
		default:
			endReadWP();
			break;
	}
}

/**
 * beginWrite() locks the appropriate locks for the thread to call write()
 * the thread must call endWrite() after writing
 */
template <class Item>
void Vec<Item>::beginWrite() {
	switch(myLockMethod) {
		case readerPref:
			beginWriteRP();
			break;
		case writerPref:
			beginWriteWP();
			break;
		case monitors:
			beginWriteM();
			break;
		default:
			beginWriteWP();
			break;
	}
}

/**
 * endWrite() unlocks the appropriate locks after the thread calls write()
 * the thread must have called beginWrite() earlier
 */
template <class Item>
void Vec<Item>::endWrite() {
	switch(myLockMethod) {
		case readerPref:
			endWriteRP();
			break;
		case writerPref:
			endWriteWP();
			break;
		case monitors:
			endWriteM();
			break;
		default:
			endWriteWP();
			break;
	}
}

// readers preference versions of locking
template <class Item>
void Vec<Item>::beginReadRP() {
	pthread_mutex_lock( &mutex );	// lock to access readcount
	readcount++;
	if (readcount == 1) 			// if first reader,
		pthread_mutex_lock( &wrt ); 	// lock Vec from writers
	pthread_mutex_unlock( &mutex ); // free readcount
}

template <class Item>
void Vec<Item>::endReadRP() {
	pthread_mutex_lock( &mutex );	// lock to access reacount
	readcount--;
	if (readcount == 0)				// if last reader,
		pthread_mutex_unlock( &wrt );	// unlock Vec for writers
	pthread_mutex_unlock( &mutex );	// free readcount
}

template <class Item>
void Vec<Item>::beginWriteRP() {
	pthread_mutex_lock( &wrt ); // lock
}

template <class Item>
void Vec<Item>::endWriteRP() {
	pthread_mutex_unlock( &wrt ); // unlock
}

// writers preference versions of locking
template <class Item>
void Vec<Item>::beginReadWP() {
	pthread_mutex_lock( &write_pending ); 	// ensures at most one reader will go before pending a write
	pthread_mutex_lock( &read_block );
	pthread_mutex_lock( &read_mutex ); 		// ensure mutal exclusion
	readers++;								// another reader
	if(readers == 1)						// synchronize with writers
		pthread_mutex_lock( &write_block );
	pthread_mutex_unlock( &read_mutex );
	pthread_mutex_unlock( &read_block );
	pthread_mutex_unlock( &write_pending );
}

template <class Item>
void Vec<Item>::endReadWP() {
	pthread_mutex_lock( &read_mutex ); 		// ensure mutual exclusion
	readers--;								// reader done
	if(readers == 0)						// enable writers
		pthread_mutex_unlock( &write_block );
	pthread_mutex_unlock( &read_mutex );
}

template <class Item>
void Vec<Item>::beginWriteWP() {
	pthread_mutex_lock( &write_mutex );		// ensure mutual exclusion
	writers++;								// another pending writer
	if(writers == 1)						// block readers
		pthread_mutex_lock( &read_block );
	pthread_mutex_unlock( &write_mutex );
	pthread_mutex_lock( &write_block );
}

template <class Item>
void Vec<Item>::endWriteWP() {
	pthread_mutex_unlock( &write_block );
	pthread_mutex_lock( &write_mutex );		// ensure mutual exclusion
	writers--;								// writer done
	if(writers == 0)						// enable readers
		pthread_mutex_unlock( &read_block );
	pthread_mutex_unlock( &write_mutex );
}

// monitors versions of locking
template <class Item>
void Vec<Item>::beginReadM() {
	pthread_mutex_lock( &mutex );
	while( numWriters > 0 ) {		// wait for readOK signal while there are writers in the data
		pthread_cond_wait( &readOK, &mutex );
	}
	numReaders++;
	pthread_mutex_unlock( &mutex );
}

template <class Item>
void Vec<Item>::endReadM() {
	pthread_mutex_lock( &mutex );
	numReaders--;
	if( numReaders == 0 ) pthread_cond_signal( &writeOK );
	pthread_mutex_unlock( &mutex );
}

template <class Item>
void Vec<Item>::beginWriteM() {
	pthread_mutex_lock( &mutex );
	while( numReaders > 0 || numWriters > 0 ) { // wait for writeOK signal while any readers or writers in the data
		pthread_cond_wait( &writeOK, &mutex );
	}
	numWriters++;
	pthread_mutex_unlock( &mutex );
}

template <class Item>
void Vec<Item>::endWriteM() {
	pthread_mutex_lock( &mutex );
	numWriters--;
//	pthread_cond_signal( &writeOK ); // alert other writers they may write
	if( numWriters == 0 )
		pthread_cond_broadcast( &readOK );  // alert other readers they may read
	pthread_mutex_unlock( &mutex );
}

// destructor
template <class Item>
Vec<Item>::~Vec() {
	delete [] myArray;
	mySize = 0;
	myArray = NULL;
	//destroy mutexes
	pthread_mutex_destroy( &mutex );
	pthread_mutex_destroy( &wrt );
	pthread_mutex_destroy( &write_mutex );
	pthread_mutex_destroy( &read_mutex );
	pthread_mutex_destroy( &write_block );
	pthread_mutex_destroy( &read_block );
	pthread_mutex_destroy( &write_pending );
	pthread_mutex_destroy( &mut );
	//destroy cond variables
	pthread_cond_destroy( &readOK );
	pthread_cond_destroy( &writeOK );
}


#endif /*VEC_H_*/