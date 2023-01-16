QT += core gui #widgets opengl
CONFIG += console

# We assume that the compiler is compatible with the C++ 11 standard.
greaterThan(QT_MAJOR_VERSION, 4){
    CONFIG         += c++11
    QT             += widgets
} else {
    QMAKE_CXXFLAGS += -std=c++0x
}

greaterThan(QT_MAJOR_VERSION, 5){
    QT             += openglwidgets
}

win32 {
    message("Windows platform...")

    INCLUDEPATH += $$PWD/Dependencies/Include
    DEPENDPATH += $$PWD/Dependencies/Include

    LIBS += -lopengl32 -lglu32

    CONFIG(release, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x64 (i.e., 64-bit) release build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x64/" -lglew32
        }
    } else: CONFIG(debug, debug|release): {
        contains(QT_ARCH, i386) {
            message("x86 (i.e., 32-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x86/" -lglew32
        } else {
            message("x64 (i.e., 64-bit) debug build")
            LIBS += -L"$$PWD/Dependencies/Lib/GL/x64/" -lglew32
        }
    }

    msvc {
      QMAKE_CXXFLAGS += -openmp -arch:AVX -D "_CRT_SECURE_NO_WARNINGS"
      QMAKE_CXXFLAGS_RELEASE *= -O2
    }
}

unix: !mac {
    message("Unix/Linux platform...")

    # for GLEW installed into /usr/lib/libGLEW.so or /usr/lib/glew.lib
    LIBS += -lGLEW -lGLU
}

mac {
    message("Macintosh platform...")

    # IMPORTANT: change the letters x, y, z in the next two lines
    # to the corresponding version numbers of the GLEW library
    # which was installed by using the command 'brew install glew'
    INCLUDEPATH += "/usr/local/Cellar/glew/x.y.z/include/"
    LIBS += -L"/usr/local/Cellar/glew/x.y.z/lib/" -lGLEW

    # the OpenGL library has to added as a framework
    LIBS += -framework OpenGL
}


FORMS += \
    GUI/MainWindow.ui \
    GUI/SideWidget.ui

HEADERS += \
    Basis/BasisFunctions.h \
    Biquartic/BiquarticArc3.h \
    Biquartic/BiquarticCompositeCurve3.h \
    Biquartic/BiquarticCompositePatches3.h \
    Biquartic/BiquarticPatch3.h \
    Biquartic/biquarticcompositecurve3.h \
    Core/Colors4.h \
    Core/Constants.h \
    Core/DCoordinates3.h \
    Core/GenericCurves3.h \
    Core/HCoordinates3.h \
    Core/Lights.h \
    Core/LinearCombination3.h \
    Core/Materials.h \
    Core/Matrices.h \
    Core/Properties.h \
    Core/RealSquareMatrices.h \
    Core/ShaderPrograms.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    Dependencies/Include/GL/glew.h \
    GUI/GLWidget.h \
    GUI/MainWindow.h \
    GUI/SideWidget.h \
    Core/Exceptions.h

SOURCES += \
    Basis/BasisFunctions.cpp \
    Biquartic/BiquarticCompositeCurve3.cpp \
    Biquartic/BiquarticCompositePatches3.cpp \
    Biquartic/BiquarticPatch3.cpp \
    Biquartic/Join.cpp \
    Biquartic/Merge.cpp \
    Biquartic/Move.cpp \
    Biquartic/biquarticarc3.cpp \
    Core/GenericCurves3.cpp \
    Core/Lights.cpp \
    Core/LinearCombination3.cpp \
    Core/Materials.cpp \
    Core/RealSquareMatrices.cpp \
    Core/ShaderPrograms.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    GUI/GLWidget.cpp \
    GUI/MainWindow.cpp \
    GUI/SideWidget.cpp \
    main.cpp

DISTFILES += \
    .gitignore \
    Dependencies/Lib/GL/x64/glew32.dll \
    Dependencies/Lib/GL/x64/glew32.lib \
    Dependencies/Lib/GL/x64/readme.txt \
    Dependencies/Lib/GL/x86/glew32.dll \
    Dependencies/Lib/GL/x86/glew32.lib \
    Dependencies/Lib/GL/x86/readme.txt
