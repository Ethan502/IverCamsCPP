PROJECT_NAME            = cameraCapture

PROJECT_DIR             = ../..
EXAMPLES_DIR            = $(PROJECT_DIR)/../..
VIMBASDK_DIR			= $(EXAMPLES_DIR)/../..
MAKE_INCLUDE_DIR        = $(CURDIR)/$(EXAMPLES_DIR)/Build/Make
BIG_INCLUDE				= $(VIMBASDK_DIR)/VimbaCPP/Source

include $(MAKE_INCLUDE_DIR)/Common.mk

CONFIG_DIR          = $(ARCH)_$(WORDSIZE)bit
BIN_FILE            = $(PROJECT_NAME)
BIN_DIR             = binary/$(CONFIG_DIR)
OBJ_DIR             = object/$(CONFIG_DIR)
BIN_PATH            = $(BIN_DIR)/$(BIN_FILE)
LIB_PATH			= $(PROJECT_DIR)/lib

all: $(BIN_PATH)

include $(MAKE_INCLUDE_DIR)/VimbaCPP.mk

include $(MAKE_INCLUDE_DIR)/VimbaImageTransform.mk

SOURCE_DIR          = $(PROJECT_DIR)/Source
INCLUDE_DIRS        = -I$(SOURCE_DIR) \
                      -I$(EXAMPLES_DIR) \
					  -I$(BIG_INCLUDE)

LIBS                = $(VIMBACPP_LIBS) \
                      $(VIMBAIMAGETRANSFORM_LIBS) \
					  -lrt

DEFINES             =

CFLAGS              = $(COMMON_CFLAGS) \
                      $(VIMBACPP_CFLAGS) \
                      $(VIMBAIMAGETRANSFORM_CFLAGS)

OBJ_FILES           = $(OBJ_DIR)/ApiController.o \
                      $(OBJ_DIR)/FrameObserver.o \
                      $(OBJ_DIR)/main.o \
					  $(OBJ_DIR)/Bitmap.o \
					  $(OBJ_DIR)/Saver.o

DEPENDENCIES        = VimbaCPP \
                      VimbaImageTransform

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(OBJ_DIR)
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -o $@ $<

$(LIB_PATH)/libfun.a: $(OBJ_DIR) 
	ar cr $(LIB_PATH)/libfun.a $(OBJ_DIR)/*.o

$(BIN_PATH): $(DEPENDENCIES) $(OBJ_FILES) $(BIN_DIR) $(LIB_PATH)/libfun.a $(LIB_PATH)/libmyVimba.a
	$(CXX) $(ARCH_CFLAGS) -lfun -lmyVimba -o $(BIN_PATH) $(OBJ_FILES) -L $(LIB_PATH) -Wl,-rpath,'$$ORIGIN'

clean:
	$(RM) binary -r -f
	$(RM) object -r -f

$(OBJ_DIR):
	$(MKDIR) -p $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR) -p $(BIN_DIR)