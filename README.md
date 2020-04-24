Configuration Manager: Use x86 (Active Win32)
Configuration Properties Advanced: Character Set: Use Multi-Byte Character Set
Configuration Properties Advanced: Use Debug Libraries: No
Configuration Properties Advanced: Whole Program Optimization: No Whole Program Optimization
Configuration Properties VC++ Directories Library Directories: $(ProjectDir)GL;$(LibraryPath)

C/C++ Preprocessor: WIN32;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;
C/C++ Language Conformance mode: No


Linker Input: OpenGL32.lib;GLu32.lib;dsound.lib;winmm.lib;glaux.lib;legacy_stdio_definitions.lib;%(AdditionalDependencies)
Linker CommandLine: OpenGL32.lib GLu32.lib GLaux.lib winmm.lib dsound.lib