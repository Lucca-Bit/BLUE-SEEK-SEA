#include "common.h"

User_Account *creat_List() {
    User_Account *head_node = (User_Account *)malloc(sizeof(User_Account));
    head_node->next = NULL;
    return head_node;
}

User_Account *creat_Node() {
    User_Account *new_node = (User_Account *)malloc(sizeof(User_Account));
    new_node->limit = 0;
    new_node->next = NULL;
    return new_node;
}

User_Account *account_data_Comparison(User_Account *head_node, char input_data[MAX_LENGTH]) {
    User_Account *tmp = head_node->next;
    while (tmp != NULL) {
        if (strcmp(input_data, tmp->user_account) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

User_Account *login(User_Account *head_node) {
    if (head_node == NULL) {
        printf("系统异常！\n");
        return NULL;
    }

    char account[MAX_LENGTH], password[MAX_LENGTH];
    int retry = 5;

    clear_input_buffer();
    while (retry > 0) {
        printf("请输入账号: ");
        get_string_input("", account, MAX_LENGTH);
        
        User_Account *user = account_data_Comparison(head_node, account);
        if (user) {
            printf("请输入密码(剩余%d次): ", retry);
            get_string_input("", password, MAX_LENGTH);
            
            if (strcmp(user->user_password, password) == 0) {
                printf("欢迎 %s！\n", account);
                return user;
            } else {
                retry--;
                if (retry > 0) printf("密码错误！\n");
            }
        } else {
            printf("账号不存在！\n");
            return NULL;
        }
    }
    printf("重试次数过多！\n");
    return NULL;
}

User_Account *add_Node(User_Account *head_node) {
    if (head_node == NULL) {
        printf("系统异常！\n");
        return NULL;
    }

    User_Account *new_node = creat_Node();
    if (new_node == NULL) {
        printf("注册失败！\n");
        return NULL;
    }

    char account[MAX_LENGTH], password1[MAX_LENGTH], password2[MAX_LENGTH];
    
    clear_input_buffer();
    while (1) {
        printf("请输入账号(字母数字组合): ");
        get_string_input("", account, MAX_LENGTH);
        
        if (strlen(account) == 0) {
            printf("账号不能为空！\n");
            continue;
        }
        break;
    }

    if (account_data_Comparison(head_node, account)) {
        printf("账号已存在！\n");
        free(new_node);
        return NULL;
    }

    while (1) {
        printf("请输入密码: ");
        get_string_input("", password1, MAX_LENGTH);
        printf("请再次输入密码: ");
        get_string_input("", password2, MAX_LENGTH);
        
        if (strcmp(password1, password2) == 0) break;
        printf("两次密码不一致！\n");
    }

    strcpy(new_node->user_account, account);
    strcpy(new_node->user_password, password1);
    new_node->next = head_node->next;
    head_node->next = new_node;
    
    printf("注册成功！\n");
    return new_node;
}

// 其他用户认证函数...
User_Account *search_Node(User_Account *head_node) {
    if (!head_node || !head_node->next) {
        printf("暂无用户数据！\n");
        return NULL;
    }

    char account[MAX_LENGTH];
    clear_input_buffer();
    printf("请输入要查询的账号: ");
    get_string_input("", account, MAX_LENGTH);

    User_Account *user = account_data_Comparison(head_node, account);
    if (user) {
        printf("账号: %s\n", user->user_account);
        printf("密码: %s\n", user->user_password);
        printf("借书数量: %d\n", user->limit);
        return user;
    } else {
        printf("账号不存在！\n");
        return NULL;
    }
}

void printf_List(User_Account *head_node) {
    if (!head_node || !head_node->next) {
        printf("暂无用户数据！\n");
        return;
    }

    User_Account *tmp = head_node->next;
    int count = 1;
    
    printf("\n=== 所有用户列表 ===\n");
    while (tmp) {
        printf("%d. 账号: %s, 密码: %s, 借书: %d本\n",
               count, tmp->user_account, tmp->user_password, tmp->limit);
        tmp = tmp->next;
        count++;
    }
}

void delete_Node(User_Account *head_node) {
    if (!head_node || !head_node->next) {
        printf("暂无用户数据！\n");
        return;
    }

    char account[MAX_LENGTH];
    clear_input_buffer();
    printf("请输入要删除的账号: ");
    get_string_input("", account, MAX_LENGTH);

    User_Account *prev = head_node;
    User_Account *curr = head_node->next;
    int found = 0;

    while (curr) {
        if (strcmp(curr->user_account, account) == 0) {
            prev->next = curr->next;
            free(curr);
            found = 1;
            printf("删除成功！\n");
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found) printf("未找到该用户！\n");
}

User_Account *destory_Node(User_Account *head_node) {
    if (!head_node) return NULL;

    User_Account *tmp = head_node->next;
    while (tmp) {
        User_Account *next = tmp->next;
        free(tmp);
        tmp = next;
    }
    head_node->next = NULL;
    printf("用户系统已清空！\n");
    return head_node;
}

User_Account *change_Node(User_Account *head_node) {
    User_Account *user = search_Node(head_node);
    if (!user) return NULL;

    char new_pass[MAX_LENGTH], confirm_pass[MAX_LENGTH];
    clear_input_buffer();

    while (1) {
        printf("请输入新密码: ");
        get_string_input("", new_pass, MAX_LENGTH);
        printf("请确认新密码: ");
        get_string_input("", confirm_pass, MAX_LENGTH);

        if (strcmp(new_pass, confirm_pass) == 0) break;
        printf("两次密码不一致！\n");
    }

    strcpy(user->user_password, new_pass);
    printf("密码修改成功！\n");
    return user;
}