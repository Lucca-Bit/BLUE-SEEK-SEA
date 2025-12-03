#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int borrow_limited = BORROW_LIMIT;
char admin[MAX_LENGTH] = ADMIN_ACCOUNT;
char admin_pass[MAX_LENGTH] = ADMIN_PASSWORD;

// 管理员登录函数
int admin_login() {
    char input_admin[MAX_LENGTH];
    char input_admin_pass[MAX_LENGTH];

    clear_input_buffer();
    printf("请输入管理员账号: ");
    get_string_input("", input_admin, MAX_LENGTH);
    printf("请输入管理员密码: ");
    get_string_input("", input_admin_pass, MAX_LENGTH);

    if (strcmp(input_admin, admin) == 0 && strcmp(input_admin_pass, admin_pass) == 0) {
        printf("管理员登录成功!\n");
        return 1;
    }
    printf("账号密码错误!\n");
    return 0;
}

// 管理员菜单
void admin_menu(Library_Book *library, User_Account *user_system) {
    int admin_first_select = 0;

    while (1) {
        printf("\n=== 管理员菜单 ===\n");
        printf("1、新增图书\n");
        printf("2、删除图书\n");
        printf("3、查询图书\n");
        printf("4、查看所有图书\n");
        printf("5、删除账号\n");
        printf("6、查询所有账号\n");
        printf("7、格式化系统\n");
        printf("8、退出管理员菜单\n");

        admin_first_select = get_valid_int_input("请选择: ", 1, 8);

        switch (admin_first_select) {
        case 1: {
            int add_select = 1;
            while (add_select == 1) {
                add_book_Node(library);
                add_select = get_valid_int_input("继续添加？1-是 2-否: ", 1, 2);
            }
            break;
        }
        case 2:
            delete_book_Node(library);
            break;
        case 3:
            search_title(library);
            break;
        case 4:
            printf_all_title(library);
            break;
        case 5:
            delete_Node(user_system);
            break;
        case 6:
            printf_List(user_system);
            break;
        case 7: {
            int format_choice = get_valid_int_input("格式化图书系统(1)还是用户系统(2)? ", 1, 2);
            if (format_choice == 1) {
                destory_book_Node(library);
                library = creat_Library();
                printf("图书系统格式化完成！\n");
            } else {
                destory_Node(user_system);
                user_system = creat_List();
                printf("用户系统格式化完成！\n");
            }
            break;
        }
        case 8:
            printf("退出管理员菜单\n");
            return;
        default:
            printf("无效选择！\n");
        }
        system_pause();
    }
}

// 用户操作菜单（修复函数名）
User_Account *user_login_success(Library_Book *head_node, User_Account *login_user) {
    system("cls");
    printf("登录成功！欢迎 %s\n", login_user->user_account);
    int second_select = 0;

    while (1) {
        printf("\n=== 用户菜单 ===\n");
        printf("1、查询图书\n");
        printf("2、借书\n");
        printf("3、还书\n");
        printf("4、查询已借阅书本\n");
        printf("5、退出登录\n");

        second_select = get_valid_int_input("请选择: ", 1, 5);

        switch (second_select) {
        case 1:
            search_title(head_node);
            break;
        case 2:
            borrow_book(head_node, login_user);
            break;
        case 3:
            return_book(head_node, login_user);
            break;
        case 4:
            borrowing_History(login_user);
            break;
        case 5:
            printf("退出登录成功！\n");
            return login_user;
        default:
            printf("无效选择！\n");
        }
        system_pause();
    }
    return login_user;
}

int main() {
    // 初始化系统
    Library_Book *library = creat_Library();
    User_Account *user_system = creat_List();

    printf("=== 广州市白云区图书馆管理系统 ===\n");

    while (1) {
        printf("\n=== 主菜单 ===\n");
        printf("1、用户登录\n");
        printf("2、用户注册\n");
        printf("3、密码找回\n");
        printf("4、管理员登录\n");
        printf("5、退出系统\n");

        int choice = get_valid_int_input("请选择操作: ", 1, 5);

        switch (choice) {
        case 1: {
            User_Account *user = login(user_system);
            if (user) {
                user_login_success(library, user);  // 修复函数调用
            }
            break;
        }
        case 2:
            add_Node(user_system);
            break;
        case 3:
            search_Node(user_system);
            break;
        case 4:
            if (admin_login()) {
                admin_menu(library, user_system);
            }
            break;
        case 5:
            // 清理资源
            destory_Node(user_system);
            destory_book_Node(library);
            printf("感谢使用广州市白云区图书馆管理系统！再见！\n");
            return 0;
        default:
            printf("无效选择！\n");
        }
        system_pause();
    }
}