// ヘッダーファイル読み込み
#include <stdio.h>
#include <ctype.h>

// プロトタイプ宣言
static void WordCount(FILE *fp, char *fname);

// 複数ファイル読み込み時の合計数をカウントする変数
int total_lines = 0;
int total_words = 0;
int total_characters = 0;

// メイン関数
int main(int argc, char **argv) {
    // コマンドライン引数の数を確認
    if(argc == 1) {
        WordCount(stdin, NULL);
        return 0;
    }

    // ファイルの数だけ繰り返す
    for(char **fname = argv + 1; *fname; fname++) {
        // ファイルを読み込み専用にしてポインタへ渡す
        FILE *fp = fopen(*fname, "r");
        // ファイル名違う、または無い場合
        if(!fp) {
            perror("fopen");
            return 1;
        }
        // コマンドラインに入力されたファイル名を引数にして関数を呼び出す
        WordCount(fp, *fname);
    }

    // ファイルを二つ以上読み込んだ場合、合計を表示
    if(argc > 2) {
        printf("行数: %2d  単語数: %2d  文字数: %2d  total\n", total_lines, total_words, total_characters);
    }

    return 0;
}

// wordCount関数
static void WordCount(FILE *fp, char *fname) {

    // ファイル読み込み用の変数をsize_t型にして宣言
    size_t readN;

    // fread用に配列を宣言
    char buff[1024];
    // バイトの宣言
    int bite = 1;

    // 行、単語、文字数を入れるための変数
    // 初期値を０とする
    int line = 0;
    int word = 0;
    int character = 0;

    // 単語判定用変数
    int wordF = 0;

    /*
    ファイルの文字を1バイトずつ読み込んでいき、０になるまで繰り返す
    */
    while((readN = fread(buff, bite, sizeof(buff), fp)) > 0) {
        // ファイルの総バイトを数えるカウンタ
        character += readN;

        // 1文字ずつループさせる
        for(size_t i = 0; i < readN; i++) {
            // 配列に格納されている文字を変数へ
            char c = buff[i];
            // 文字が改行文字であれば
            if(c == '\n') {
                // 行数をふやす
                line++;
            }
            // wordFが1で且つ、文字が空白文字なら
            if(wordF == 1 && isspace(c)) {
                // wordFを0に
                wordF = 0;

            /* 
            wordFが0(前の文字が空白文字)のときに
            抜き出した文字が空白文字ではないならwordFを1にする
            // つまり、空白の次に文字がある場合、単語カウントを1増やす
            */
            } else if(wordF == 0 && !isspace(c)) {
                wordF = 1;
                word++;
            }
        }
    }

    // ファイル名がある場合
    if(fname) {
        printf("行数: %2d  単語数: %2d  文字数: %d  ファイル名: %2s\n", line, word, character, fname);
    // ファイル名がない場合の結果表示(stdinから呼ぶ場合)
    } else {
        printf("行数: %2d  単語数: %2d  文字数: %2d\n", line, word, character);
    }

    // 今までの合計数をカウント
    total_lines += line;
    total_words += word;
    total_characters += character;

    // フォルダを閉じる
    fclose(fp);
}