CONFIG(debug, debug|release){
    contains(QT_ARCH, i386) {
        DESTDIR = $$PWD/x86/debug
    }else {
        DESTDIR = $$PWD/x64/debug
    }
}else{
    contains(QT_ARCH, i386) {
        DESTDIR = $$PWD/x86/release
    }else {
        DESTDIR = $$PWD/x64/release
    }
}

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

INCLUDEPATH += $$PWD/include
