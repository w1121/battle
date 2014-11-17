# makefile for System: Model

sctAUTOCFGDEP =
sctCOMPFLAGS = -DXUSE_GENERIC_FUNC

!include $(SCTDIR)\make.opt

default: Model$(sctEXTENSION)

Model$(sctEXTENSION): \
  Model$(sctOEXTENSION) \
  ctypes$(sctOEXTENSION) \
  BasicTypes$(sctOEXTENSION) \
  ExternalFunction$(sctOEXTENSION) \
  MapFunctions$(sctOEXTENSION) \
  ZSWrap$(sctOEXTENSION) \
  ZSocketData$(sctOEXTENSION) \
  ZSocket$(sctOEXTENSION) \
  ZSSocket$(sctOEXTENSION) \
  ZCSocket$(sctOEXTENSION) \
  $(sctLINKKERNELDEP)
	$(sctLD) @<<
	$(sctLDFLAGS)
	 Model$(sctOEXTENSION) ctypes$(sctOEXTENSION) BasicTypes$(sctOEXTENSION) ExternalFunction$(sctOEXTENSION) MapFunctions$(sctOEXTENSION) ZSWrap$(sctOEXTENSION) ZSocketData$(sctOEXTENSION) ZSocket$(sctOEXTENSION) ZSSocket$(sctOEXTENSION) ZCSocket$(sctOEXTENSION) $(sctLINKKERNEL)
	/OUT:Model$(sctEXTENSION)
<<

Model$(sctOEXTENSION): \
  Model.c \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS)
	$(sctIFDEF)
	/FoModel$(sctOEXTENSION)
	Model.c
<<

ctypes$(sctOEXTENSION): \
  ctypes.c \
  BasicTypes.h \
  ctypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS)
	$(sctIFDEF)
	/Foctypes$(sctOEXTENSION)
	ctypes.c
<<

BasicTypes$(sctOEXTENSION): \
  BasicTypes.c \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS)
	$(sctIFDEF)
	/FoBasicTypes$(sctOEXTENSION)
	BasicTypes.c
<<

ExternalFunction$(sctOEXTENSION): \
  ..\..\Source\ExternalFunction.c \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS)
	$(sctIFDEF)
	/FoExternalFunction$(sctOEXTENSION)
	..\..\Source\ExternalFunction.c
<<

MapFunctions$(sctOEXTENSION): \
  ..\..\Source\MapFunctions.c \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS)
	$(sctIFDEF)
	/FoMapFunctions$(sctOEXTENSION)
	..\..\Source\MapFunctions.c
<<

ZSocketData$(sctOEXTENSION): \
  ..\..\Source\ZSocket\ZSocketData.cpp \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS) -GX
	$(sctIFDEF)
	/FoZSocketData$(sctOEXTENSION)
	..\..\Source\ZSocket\ZSocketData.cpp
<<

ZSocket$(sctOEXTENSION): \
  ..\..\Source\ZSocket\ZSocket.cpp \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS) -GX
	$(sctIFDEF)
	/FoZSocket$(sctOEXTENSION)
	..\..\Source\ZSocket\ZSocket.cpp
<<

ZSSocket$(sctOEXTENSION): \
  ..\..\Source\ZSocket\ZSSocket.cpp \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS) -GX
	$(sctIFDEF)
	/FoZSSocket$(sctOEXTENSION)
	..\..\Source\ZSocket\ZSSocket.cpp
<<

ZCSocket$(sctOEXTENSION): \
  ..\..\Source\ZSocket\ZCSocket.cpp \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS) -GX
	$(sctIFDEF)
	/FoZCSocket$(sctOEXTENSION)
	..\..\Source\ZSocket\ZCSocket.cpp
<<

ZSWrap$(sctOEXTENSION): \
  ..\..\Source\ZSWrap\ZSWrap.cpp \
  ctypes.h \
  BasicTypes.h
	$(sctCC) @<<
	$(sctCPPFLAGS) $(sctCCFLAGS) -GX
	$(sctIFDEF)
	/FoZSWrap$(sctOEXTENSION)
	..\..\Source\ZSWrap\ZSWrap.cpp
<<
