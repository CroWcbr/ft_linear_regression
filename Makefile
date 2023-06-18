.DEFAULT_GOAL	:=	all

NAME_PREDICT	=	predict
NAME_TRAIN		=	train
NAME_BONUS		=	bonus

CC				=	c++ -std=c++17
CFLAGS			=	-Wall -Werror -Wextra

SRC_DIR			=	./src
OBJ_DIR			=	./obj
PLOT_DIR		=	./plot

SRC_P			=	predict.cpp
SRC_T			=	train.cpp \
					read_data.cpp \
					solve_linear_regression.cpp \
					write_model.cpp
SRC_B			=	bonus.cpp \
					read_data.cpp \
					solve_linear_regression.cpp

OBJ_P			=	$(addprefix $(OBJ_DIR)/, $(SRC_P:.cpp=.o))
OBJ_T			=	$(addprefix $(OBJ_DIR)/, $(SRC_T:.cpp=.o))
OBJ_B			=	$(addprefix $(OBJ_DIR)/, $(SRC_B:.cpp=.o))

$(OBJ_P)		: 	| $(OBJ_DIR)
$(OBJ_T)		: 	| $(OBJ_DIR)
$(OBJ_B)		: 	| $(OBJ_DIR)

$(OBJ_DIR)		:
					@mkdir -p $(OBJ_DIR)
			
$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp
					$(CC) $(CFLAGS) -c $< -o $@

RM_DIR			=	rm -rf
RM_FILE			=	rm -f

#COLORS
C_NO			=	"\033[00m"
C_OK			=	"\033[32m"
C_GOOD			=	"\033[32m"

#DEBUG
SUCCESS			=	$(C_GOOD)SUCCESS$(C_NO)
OK				=	$(C_OK)OK$(C_NO)

all				:	$(NAME_PREDICT) $(NAME_TRAIN) $(NAME_BONUS) 

$(NAME_PREDICT)	:	$(OBJ_P)
					$(CC) $(CFLAGS) $(OBJ_P) -o $(NAME_PREDICT)
					@echo "\tCompiling...\t" [ $(NAME_PREDICT) ] $(SUCCESS)

$(NAME_TRAIN)	:	$(OBJ_T)
					$(CC) $(CFLAGS) $(OBJ_T) -o $(NAME_TRAIN)
					@echo "\tCompiling...\t" [ $(NAME_TRAIN) ] $(SUCCESS)

$(NAME_BONUS)	:	$(OBJ_B)
					$(CC) $(CFLAGS) $(OBJ_B) -o $(NAME_BONUS)
					@echo "\tCompiling...\t" [ $(NAME_BONUS) ] $(SUCCESS)

MODEL_PATH		=	./data/model.csv
clean			:
					@$(RM_DIR) $(PLOT_DIR)
					@echo "\tCleaning...\t" [ $(PLOT_DIR) ] $(OK)
					@$(RM_FILE) $(MODEL_PATH)
					@echo "\tCleaning...\t" [ $(MODEL_PATH) ] $(OK)
					@$(RM_DIR) $(OBJ_DIR)
					@echo "\tCleaning...\t" [ $(OBJ_DIR) ] $(OK)

fclean			:	clean
					@$(RM_FILE) $(NAME_PREDICT)
					@$(RM_FILE) $(NAME_TRAIN)
					@$(RM_FILE) $(NAME_BONUS)
					@echo "\tDeleting...\t" [ $(NAME_PREDICT) ] $(OK)
					@echo "\tDeleting...\t" [ $(NAME_TRAIN) ] $(OK)
					@echo "\tDeleting...\t" [ $(NAME_BONUS) ] $(OK)

re				:	fclean all

.PHONY			:	all, clean, fclean, re
