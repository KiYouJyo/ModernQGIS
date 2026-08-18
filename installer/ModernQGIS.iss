#ifndef MyAppVersion
#define MyAppVersion "0.3.0"
#endif
#ifndef StageDir
#define StageDir "..\dist\stage"
#endif
#ifndef OutputDir
#define OutputDir "..\dist"
#endif

[Setup]
AppId={{D6810131-97E0-4E4A-BBE5-8CF59E9A1B38}
AppName=ModernQGIS ShellPreview
AppVersion={#MyAppVersion}
AppVerName=ModernQGIS ShellPreview {#MyAppVersion}
AppPublisher=ModernQGIS Contributors
DefaultDirName={localappdata}\Programs\ModernQGIS
DefaultGroupName=ModernQGIS
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputDir={#OutputDir}
OutputBaseFilename=ModernQGIS-ShellPreview-v{#MyAppVersion}-windows-x64-setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
SetupLogging=yes
UninstallDisplayName=ModernQGIS ShellPreview {#MyAppVersion}

[Files]
Source: "{#StageDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\ModernQGIS ShellPreview"; Filename: "{app}\modernqgis-sandbox.exe"; WorkingDir: "{app}"
Name: "{userdesktop}\ModernQGIS ShellPreview"; Filename: "{app}\modernqgis-sandbox.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional shortcuts:"; Flags: unchecked

[Run]
Filename: "{app}\modernqgis-sandbox.exe"; Description: "Launch ModernQGIS ShellPreview"; Flags: nowait postinstall skipifsilent
