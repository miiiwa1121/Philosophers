# ============================================================================== #
#                                   Variables                                    #
# ============================================================================== #

# NAME: 生成される実行ファイルの名前
NAME = philo

# CC: 使用するコンパイラ
CC = cc

# CFLAGS: コンパイル時に適用されるフラグ
# -Wall -Wextra: ほとんど全ての警告を有効にする
# -Werror:       全ての警告をエラーとして扱い、コンパイルを停止させる
# -Iinclude:     ヘッダーファイルを探すディレクトリに'include'を追加する
# -pthread:      スレッド関連の関数をコンパイル・リンクするために必要
CFLAGS = -Wall -Wextra -Werror -Iinclude -pthread

# SRCS_DIR: ソースファイル(.c)が格納されているディレクトリ
SRCS_DIR = src
# OBJS_DIR: オブジェクトファイル(.o)を格納するディレクトリ
OBJS_DIR = obj

# SRCS_LIST: プロジェクトに含まれる全ソースファイルのリスト
SRCS_LIST = main.c \
            init.c \
            simulation.c \
            actions.c \
            monitor.c \
            utils.c

# SRCS: SRCS_LISTの各ファイル名の先頭に、ソースディレクトリのパスを追加する
# 例: src/main.c src/init.c ...
SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_LIST))

# OBJS: SRCS_LISTの各ファイル名の拡張子を.cから.oに変更し、
#       先頭にオブジェクトディレクトリのパスを追加する
# 例: obj/main.o obj/init.o ...
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS_LIST:.c=.o))

# RM: ファイルを削除するコマンド
RM = rm -f

# ============================================================================== #
#                                     Rules                                      #
# ============================================================================== #

# all: デフォルトのターゲット。'make'とだけ打った時に実行される。
#      $(NAME)の生成に依存する。
all: $(NAME)

# $(NAME): 最終的な実行ファイルを生成するルール。
#          全てのオブジェクトファイル($(OBJS))に依存する。
#          オブジェクトファイルをリンクして実行ファイルを作成する。
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# $(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
#   ソースファイル(.c)からオブジェクトファイル(.o)を生成するためのパターンルール。
#   '| $(OBJS_DIR)' は「順序のみの依存関係」を示し、.oファイルを作る前に
#   $(OBJS_DIR)が存在することを保証する。
#
#   コマンド内の自動変数:
#   $<: 依存関係リストの最初のファイル名 (例: src/main.c)
#   $@: ターゲットのファイル名 (例: obj/main.o)
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJS_DIR): オブジェクトディレクトリを作成するルール。
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# clean: オブジェクトディレクトリを削除し、コンパイル時の中間ファイルを一掃する。
clean:
	$(RM) -r $(OBJS_DIR)

# fclean: cleanルールを実行した後、最終的な実行ファイルも削除する。
fclean: clean
	$(RM) $(NAME)

# re: fcleanで全てを削除した後、allで再コンパイルする。
re: fclean all

# .PHONY: all, clean, fclean, reという名前のファイルが万が一存在しても、
#         このルールが正しくコマンドとして実行されるように指定する。
.PHONY: all clean fclean re