#define MyAppName "TSGL"
#define MyAppVerName "TSGL 1.0"
#define MyAppPublisher "Calvin College"
#define MyAppURL "cs.calvin.edu"

#include <idp.iss>

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={src}
DefaultGroupName={#MyAppName}
OutputBaseFilename=tsgl-setup
OutputDir=.
Compression=lzma
SolidCompression=true
CreateAppDir=true
ShowLanguageDialog=yes
Uninstallable = no
DirExistsWarning = no

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Code]
type 
  AllString = array[1..6] of String;

var
  ProgPage : TOutputProgressWizardPage;
  dpaths:      AllString;
  dfiles:      AllString;
  msbuildpath: String;
  appdir:      String;
  unzipcmd:    String;
  untarcmd:    String;
  cmakecmd:    String;
  msbuildcmd:  String;

procedure InitializeWizard();
var
  i:        Integer;
  file:     String;
  qpos:     Integer;
  A: AnsiString;
  S: String;
begin
 //Figure out where VS 2012's MSBuild is located; exit if we don't have VS 2012
 if not RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\MSBuild\ToolsVersions\4.0', 'MSBuildToolsPath', msbuildpath) then begin
   MsgBox('You need Visual Studio 2012 to use this installer.',mbError,MB_OK);
   Abort();
 end;
 Log('MSBuild path: ' + msbuildpath);

  //From: http://stackoverflow.com/questions/23459059/how-to-show-progress-during-preparetoinstall
  //The string msgWizardPreparing has the macro '[name]' inside that I have to replace with the name of my app, stored in a define constant of my script.
  S := SetupMessage(msgPreparingDesc); 
  StringChange(S, '[name]', '{#MyAppName}');
  A := S;
  ProgPage := CreateOutputProgressPage(SetupMessage(msgWizardPreparing), A);

 //Populate dpaths with the urls of the files we need
 //(Note: SourceForge Links automatically redirect to mirrors, and GitHub has no mirrors)
 dpaths[1] := 'http://downloads.sourceforge.net/project/gnuwin32/freetype/2.3.5-1/freetype-2.3.5-1-bin.zip';
 dpaths[2] := 'http://downloads.sourceforge.net/project/gnuwin32/freetype/2.3.5-1/freetype-2.3.5-1-dep.zip';
 dpaths[3] := 'http://downloads.sourceforge.net/project/glew/glew/1.12.0/glew-1.12.0.zip';
 dpaths[4] := 'https://github.com/glfw/glfw/releases/download/3.1.1/glfw-3.1.1.zip';
 dpaths[5] := 'https://github.com/nothings/stb/archive/master.zip';
 dpaths[6] := 'http://downloads.sourceforge.net/project/freeglut/freeglut/3.0.0/freeglut-3.0.0.tar.gz';

 //Get the filenames from those paths
 for i := 1 to 6 do begin
   file := ExtractFileName(dpaths[i]);
   qpos := Pos('?',file);
   if (qpos > 0) then begin
     dfiles[i] := Copy(file,0,qpos-1);
   end
   else begin
     dfiles[i] := file;
   end;
 end;

 //Add urls for a few miscellaneous tools we'll be using (unzip, 7zip, and cmake)
 idpAddFile('http://stahlworks.com/dev/unzip.exe', expandconstant('{tmp}\unzip.exe'));
 idpAddFile('http://www.7-zip.org/a/7za920.zip', expandconstant('{tmp}\7za920.zip'));
 idpAddFile('http://www.cmake.org/files/v3.2/cmake-3.2.3-win32-x86.zip', expandconstant('{tmp}\cmake-3.2.3-win32-x86.zip'));
 //Add the files we specified above
 for i := 1 to 6 do begin
   idpAddFile(dpaths[i],expandconstant('{tmp}\') + dfiles[i]);
 end;

 //Start the download after the "Ready to install" screen is shown
 idpDownloadAfter(wpReady);
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  //If the downloader page was just shown, then...
  if CurPageID = IDPForm.Page.ID then
  begin
    //Show the detail components...
    idpShowDetails(True);
    //...and hide the details button
    IDPForm.DetailsButton.Visible := False;
  end;
end;

//Helper procedures
procedure Run(const Cmd, Par, Cwd: String); var R: Integer; begin Exec(Cmd, Par, Cwd, SW_SHOW, ewWaitUntilTerminated, R); end;
procedure Move(const Src, Dest: String); begin RenameFile(appdir + Src,appdir + Dest); end;
procedure Del(const Src: String); begin DeleteFile(appdir + Src); end;
procedure DelDir(const Src: String); begin DelTree(appdir + Src,True,True,True); end;
procedure Unzip(const Src, Dest: String); begin Run(unzipcmd, appdir + Src + ' -d ' + appdir + Dest, ''); end;
procedure Make(const Src: String); begin Run(cmakecmd, '-G"Visual Studio 11 2012" ' + appdir + Src, appdir + Src); end;
procedure Build(const Src: String); begin Run(msbuildcmd, appdir + Src + ' /p:VisualStudioVersion=11.0 /p:PlatformToolset=v110',''); end;
procedure Untar(const Src: String); var S: String; begin
  Run(untarcmd, 'x ' + appdir + Src, '');
  S := Copy(Src,1,Length(Src)-3);
  Run(untarcmd, 'x ' + appdir + S, '');
  Del(S);  //Delete intermediate .tar file 
end;

function PrepareToInstall(var NeedsRestart: Boolean): String;
var
    ResultCode:   Integer;
    i:           Integer;
    tmpdir:      String;
begin
  ProgPage.SetProgress(0, 100);
  ProgPage.Show;
  try begin
  // First preinstallation step: suppose to kill currently app you are going to update 
    ProgPage.SetText(('Creating directories'),'');
  //Establish some useful commands
    msbuildcmd := msbuildpath + 'MSBuild.exe';
    appdir := expandconstant('{app}\');
    tmpdir := expandconstant('{tmp}\');
    unzipcmd := appdir + 'unzip.exe';
    untarcmd := appdir + '7za.exe';
    cmakecmd := appdir + 'cmake\bin\cmake.exe';
  //Create some useful subdirectories
    CreateDir(appdir);  //Not always automatically created...
    CreateDir(appdir + 'lib');
    CreateDir(appdir + 'include');
    CreateDir(appdir + 'ziplibs');
  //Copy files from tmp directory to working directory
    ProgPage.SetText(('Copying files'),'');
    ProgPage.SetProgress(5, 100);
    FileCopy(tmpdir + 'unzip.exe',unzipcmd,false);
    FileCopy(tmpdir + '7za920.zip',appdir + '7za920.zip',false);
    FileCopy(tmpdir + 'cmake-3.2.3-win32-x86.zip',appdir + 'cmake-3.2.3-win32-x86.zip',false);
    for i := 1 to 6 do begin
      FileCopy(tmpdir + dfiles[i],appdir + dfiles[i],false);
    end;
  //Unpack 7zip
    ProgPage.SetText(('Unpacking 7zip'),'');
    ProgPage.SetProgress(10, 100);
    Unzip('7za920.zip','');
    Del('7-zip.chm');
    Del('license.txt');
    Del('readme.txt');
    Del('7za920.zip');
  //Unpack CMake
    ProgPage.SetText(('Unpacking Cmake'),'');
    ProgPage.SetProgress(15, 100);
    Unzip('cmake-3.2.3-win32-x86.zip','');
    Move('cmake-3.2.3-win32-x86','cmake');
    Del('cmake-3.2.3-win32-x86.zip');
  //[1] Unpack and move files from Freetype Binary Zip
    ProgPage.SetText(('Installing Freetype'),'');
    ProgPage.SetProgress(20, 100);
    Unzip(dfiles[1],'freetype-2.3.5-1-bin');
    Move(dfiles[1],'ziplibs\' + dfiles[1]);
    Move('freetype-2.3.5-1-bin\bin\freetype6.dll','lib\freetype6.dll');
    Move('freetype-2.3.5-1-bin\include\ft2build.h','include\ft2build.h');
    Move('freetype-2.3.5-1-bin\include\freetype2\freetype','include\freetype');
    Move('freetype-2.3.5-1-bin\lib\freetype.lib','lib\freetype.lib');
    DelDir('freetype-2.3.5-1-bin');
  //[2] Unpack and move files from Freetype Dependency Zip
    Unzip(dfiles[2],'freetype-2.3.5-1-dep');
    Move(dfiles[2],'ziplibs\' + dfiles[2]);
    Move('freetype-2.3.5-1-dep\bin\zlib1.dll','lib\zlib1.dll');
    DelDir('freetype-2.3.5-1-dep');
  //[3] Unpack and move files from GLEW
    ProgPage.SetText(('Installing GLEW'),'');
    ProgPage.SetProgress(30, 100);
    Unzip(dfiles[3], '');
    Move(dfiles[3],'ziplibs\' + dfiles[3]);
    Make('glew-1.12.0');
    ProgPage.SetProgress(33, 100);
    Build('glew-1.12.0\build\vc12\glew_static.vcxproj');
    Move('glew-1.12.0\lib\Debug\Win32\glew32sd.lib','lib\glew32sd.lib');
    Move('glew-1.12.0\include\GL','include\GL');
    DelDir('glew-1.12.0');
  //[4] Unpack and move files from GLFW
    ProgPage.SetText(('Installing GLFW'),'');
    ProgPage.SetProgress(40, 100);
    Unzip(dfiles[4],'');
    Move(dfiles[4],'ziplibs\' + dfiles[4]);
    Make('glfw-3.1.1');
    ProgPage.SetProgress(45, 100);
    Build('glfw-3.1.1\GLFW.sln');
    Move('glfw-3.1.1\src\Debug\glfw3.lib','lib\glfw3.lib');
    Move('glfw-3.1.1\include\GLFW','include\GLFW');
    DelDir('glfw-3.1.1');
  //[5] Unpack and move files from STB
    ProgPage.SetText(('Installing STB'),'');
    ProgPage.SetProgress(60, 100);
    Unzip(dfiles[5],'');
    Move(dfiles[5],'ziplibs\' + dfiles[5]);
    Move('stb-master','include\stb');
  //[6] Unpack and move files from FreeGLUT
    ProgPage.SetText(('Installing FreeGLUT'),'');
    ProgPage.SetProgress(65, 100);
    Untar(dfiles[6]);
    Move(dfiles[6],'ziplibs\' + dfiles[6]);
    Make('freeglut-3.0.0');
    ProgPage.SetProgress(75, 100);
    Build('freeglut-3.0.0\freeglut.sln');
    Move('freeglut-3.0.0\lib\Debug\freeglutd.lib','lib\freeglutd.lib');
    //(Manually moving these because the "GL" directory was already created by GLEW)
    Move('freeglut-3.0.0\include\GL\freeglut.h','include\GL\freeglut.h');
    Move('freeglut-3.0.0\include\GL\freeglut_ext.h','include\GL\freeglut_ext.h');
    Move('freeglut-3.0.0\include\GL\freeglut_std.h','include\GL\freeglut_std.h');
    Move('freeglut-3.0.0\include\GL\glut.h','include\GL\glut.h');
    DelDir('freeglut-3.0.0');
  //Copy the remaining files in the windows folder
    ProgPage.SetText(('Copying Visual Studio Files'),'');
    ProgPage.SetProgress(95, 100);
	Move('windows\readmetests.txt','readmetests.txt');
	Move('windows\runtests.bat','runtests.bat');
    Move('windows\testProperties.props','testProperties.props');
    Move('windows\tsgl.sln','tsgl.sln');
    Move('windows\tsgl.v11.suo','tsgl.v11.suo');
    Move('windows\tsgl.vcxproj','tsgl.vcxproj');
    Move('windows\tsgl.vcxproj.filters','tsgl.vcxproj.filters');
    Move('windows\tsgl.vcxproj.user','tsgl.vcxproj.user');
    Move('windows\vs2012','vs2012');
  //Cleanup
    ProgPage.SetText(('Cleaning Up'),'');
    ProgPage.SetProgress(99, 100);
    Del('unzip.exe');
    Del('7za.exe');
    DelDir('cmake');
    DelDir('windows');
  end;
  finally
    ProgPage.Hide;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  i:           Integer;
  tmpdir:      String;
begin
 if CurStep = ssPostInstall  then
 begin
  if MsgBox('Keep zipped libraries?',
    mbConfirmation, MB_YESNO) = IDNO
  then
    DelDir('ziplibs');
 end;
end;
