# -----------------------------------------------------------------------------#
# Program Names                                                                #
# -----------------------------------------------------------------------------#

# Program names for server and client executables
NAME_SERVER		:= snake_server
NAME_CLIENT		:= snake_client

# -----------------------------------------------------------------------------#
# Directories                                                                  #
# -----------------------------------------------------------------------------#

# Directories for source files and headers
SERVER_SRC_DIR	:= server
CLIENT_SRC_DIR	:= client
INC_DIR			:= includes
OBJ_DIR			:= obj

# -----------------------------------------------------------------------------#
# Sources
# -----------------------------------------------------------------------------#

# Source files for server and client
SERVER_SRCS		:= $(SERVER_SRC_DIR)/snake_server.c
CLIENT_SRCS		:= $(CLIENT_SRC_DIR)/snake_client.c

# -----------------------------------------------------------------------------#
# Objects                                                                      #
# -----------------------------------------------------------------------------#

# Object files generated from sources (stored under obj/server/ and obj/client/)
SERVER_OBJS		:= $(SERVER_SRCS:$(SERVER_SRC_DIR)/%.c=$(OBJ_DIR)/server/%.o)
CLIENT_OBJS		:= $(CLIENT_SRCS:$(CLIENT_SRC_DIR)/%.c=$(OBJ_DIR)/client/%.o)

# -----------------------------------------------------------------------------#
# Dependencies                                                                 #
# -----------------------------------------------------------------------------#

# Dependency files generated alongside object files
SERVER_DEPS		:= $(SERVER_OBJS:.o=.d)
CLIENT_DEPS		:= $(CLIENT_OBJS:.o=.d)

# -----------------------------------------------------------------------------#
# Compiler and Flags                                                           #
# -----------------------------------------------------------------------------#

# Compiler to use
CC				:= cc
# Preprocessor flags (include directories, dependency generation)
CPPFLAGS		:= -I$(INC_DIR) -MMD -MP
# Compiler flags (warnings, errors)
CFLAGS			:= -Wall -Wextra -Werror
# Linker flags (e.g., -lm -lpthread if needed)
LDFLAGS			:= 
# Command to remove files/directories
RM				:= rm -rf

# -----------------------------------------------------------------------------#
# Build Rules                                                                  #
# -----------------------------------------------------------------------------#

# Default target: build both server and client
all: $(NAME_SERVER) $(NAME_CLIENT)

# Link server executable from object files
$(NAME_SERVER): $(SERVER_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Link client executable from object files
$(NAME_CLIENT): $(CLIENT_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compile server source files into object files
$(OBJ_DIR)/server/%.o: $(SERVER_SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile client source files into object files
$(OBJ_DIR)/client/%.o: $(CLIENT_SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------#
# Clean                                                                        #
# -----------------------------------------------------------------------------#

# Remove all compiled objects and dependency files
clean:
	$(RM) $(OBJ_DIR)

# Remove executables in addition to objects
fclean: clean
	$(RM) $(NAME_SERVER) $(NAME_CLIENT)

# Rebuild everything from scratch
re: fclean all

# -----------------------------------------------------------------------------#
# Include Dependencies                                                         #
# -----------------------------------------------------------------------------#

# Include dependency files to automatically track header changes
-include $(SERVER_DEPS) $(CLIENT_DEPS)

# -----------------------------------------------------------------------------#
# Phony Targets                                                                #
# -----------------------------------------------------------------------------#

# Declare phony targets so make doesn’t confuse them with files
.PHONY: all clean fclean re
