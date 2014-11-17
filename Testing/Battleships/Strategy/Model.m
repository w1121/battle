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
  $(sctLINKKERNELDEP)
	$(sctLD) @<<
	$(sctLDFLAGS)
	 Model$(sctOEXTENSION) ctypes$(sctOEXTENSION) BasicTypes$(sctOEXTENSION) ExternalFunction$(sctOEXTENSION) MapFunctions$(sctOEXTENSION) $(sctLINKKERNEL)
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
