#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH 20
#define BORROW_LIMIT 5
#define ADMIN_ACCOUNT "admin"
#define ADMIN_PASSWORD "123456a"

typedef struct BOOKS {
    char book_title[MAX_LENGTH];
    char author[MAX_LENGTH];
    char publishing_house[MAX_LENGTH];
    long year;
    int amount;
    struct BOOKS *next;
} Library_Book;

typedef struct USER_ACCOUNT {
    char user_account[MAX_LENGTH];
    char user_password[MAX_LENGTH];
    char borrowed_books[BORROW_LIMIT][MAX_LENGTH];
    int limit;
    struct USER_ACCOUNT *next;
} User_Account;

extern int borrow_limited;
extern char admin[];
extern char admin_pass[];

// 工具函数声明
void clear_input_buffer();
int get_valid_int_input(const char *prompt, int min, int max);
void get_string_input(const char *prompt, char *buffer, int size);
void system_pause();

// 用户认证函数声明
User_Account *creat_List();
User_Account *creat_Node();
User_Account *account_data_Comparison(User_Account *head_node, char input_data[MAX_LENGTH]);
User_Account *login(User_Account *head_node);
User_Account *add_Node(User_Account *head_node);
User_Account *search_Node(User_Account *head_node);
void printf_List(User_Account *head_node);
void delete_Node(User_Account *head_node);
User_Account *destory_Node(User_Account *head_node);
User_Account *change_Node(User_Account *head_node);  // 添加缺失的声明

// 图书管理函数声明
Library_Book *creat_Library();
Library_Book *creat_book_Node();
Library_Book *add_book_Node(Library_Book *head_node);
Library_Book *search_title(Library_Book *head_node);
User_Account *borrow_book(Library_Book *head_node, User_Account *login_user);
Library_Book *return_book(Library_Book *head_node, User_Account *login_user);
void printf_all_title(Library_Book *head_node);
void borrowing_History(User_Account *login);
Library_Book *delete_book_Node(Library_Book *head_node);
Library_Book *destory_book_Node(Library_Book *head_node);

// 主程序函数声明
int admin_login();
void admin_menu(Library_Book *library, User_Account *user_system);
User_Account *user_login_success(Library_Book *head_node, User_Account *login_user);  // 添加缺失的声明

#endif