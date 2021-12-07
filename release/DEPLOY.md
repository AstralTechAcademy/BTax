
1. Build with cmake and make, as a normal C++ App and create the binary.
2. Create folder realese (mkdir release)
3. Copy the binary into release (cp <binary> release/)
4. Create appdir directory and files used to create an appImage Binary for linux distributions

        cd release && mkdir appdir && mkdir appdir/usr && \
        mkdir appdir/usr/bin && \ 
        cp <binary> appdir/usr/binary &&
        ln -s usr/bin/<binary> AppRun

5. Create .desktop file following this template

        [Desktop Entry]
        Type=Application
        Name=<AppName>
        Exec=<AppExecName>
        Icon=<AppIconName_No_Extension>

6. Copy all necessary files into appdir
        
        cp <icon_file> appdir && cp <name>.desktop appdir


7. In this step appdir directory should have this structure:

    - appdir
        - usr
            - bin
                - binary_file
        - AppRun
        - *.desktop
        - icon.png

8. Run linuxdeploy since release folder
        
        ./linuxdeployqt-5-x86_64.AppImage appdir/<name>.desktop \ 
        -appimage \ 
        -qmake="<path_qt>/qmake" \ 
        -qmldir="<path_qt>/qml/"

    Output Example

        [.........]
        
        Creating 4.0 filesystem on Broker-x86_64.AppImage, block size 131072.
        [=================================================================================================\] 2247/2247 100%

        Exportable Squashfs 4.0 filesystem, gzip compressed, data block size 131072
                compressed data, compressed metadata, compressed fragments, compressed xattrs
                duplicates are removed
        Filesystem size 46605.32 Kbytes (45.51 Mbytes)
                34.15% of uncompressed filesystem size (136477.36 Kbytes)
        Inode table size 16735 bytes (16.34 Kbytes)
                34.45% of uncompressed inode table size (48582 bytes)
        Directory table size 12916 bytes (12.61 Kbytes)
                38.34% of uncompressed directory table size (33689 bytes)
        Number of duplicate files found 236
        Number of inodes 1386
        Number of files 1309
        Number of fragments 77
        Number of symbolic links  1
        Number of device nodes 0
        Number of fifo nodes 0
        Number of socket nodes 0
        Number of directories 76
        Number of ids (unique uids + gids) 1
        Number of uids 1
                root (0)
        Number of gids 1
                root (0)
        Embedding ELF...
        Marking the AppImage as executable...
        Embedding MD5 digest
        Success


9.  Into release folder should appears .AppImage file


#
# BROKER EXAMPLE

5. Create .desktop file following this template

        [Desktop Entry]
        Type=Application
        Name=Broker
        Exec=usr/bin/Broker
        Icon=shiba
7. In this step appdir directory should have this structure:

    - appdir
        - usr
            - bin
                - Broker
        - AppRun
        - Brokee.desktop
        - shiba.png        

8. Run linuxdeploy since release folder
        
        ./linuxdeployqt-5-x86_64.AppImage appdir/Broker.desktop \ 
        -appimage \ 
        -qmake="/home/gabridc/Qt/5.15.2/gcc_64/bin/qmake" \ 
        -qmldir="/home/gabridc/Qt/5.15.2/gcc_64/qml/"
       