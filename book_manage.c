#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

Library_Book *creat_Library()
{
    Library_Book *head_node = (Library_Book *)malloc(sizeof(Library_Book));
    head_node->next = NULL;
    return head_node;
}

Library_Book *creat_book_Node()
{
    Library_Book *new_node = (Library_Book *)malloc(sizeof(Library_Book));
    new_node->next = NULL;
    return new_node;
}

Library_Book *add_book_Node(Library_Book *head_node)
{
    if (!head_node)
        return NULL;

    Library_Book *new_book = creat_book_Node();
    if (!new_book)
    {
        printf("新增失败！\n");
        return NULL;
    }

    char book_name[MAX_LENGTH], writer[MAX_LENGTH], press[MAX_LENGTH];
    long publish_year;
    int book_count;

    clear_input_buffer();
    printf("请输入书名: ");
    get_string_input("", book_name, MAX_LENGTH);
    printf("请输入作者: ");
    get_string_input("", writer, MAX_LENGTH);
    printf("请输入出版社: ");
    get_string_input("", press, MAX_LENGTH);
    printf("请输入出版年份: ");
    scanf("%ld", &publish_year);
    printf("请输入数量: ");
    scanf("%d", &book_count);
    clear_input_buffer();

    // 检查是否已存在
    Library_Book *current = head_node->next;
    while (current)
    {
        if (strcmp(book_name, current->book_title) == 0 && publish_year == current->year)
        {
            printf("该书已存在！\n");
            free(new_book);
            return NULL;
        }
        current = current->next;
    }

    strcpy(new_book->book_title, book_name);
    strcpy(new_book->author, writer);
    strcpy(new_book->publishing_house, press);
    new_book->year = publish_year;
    new_book->amount = book_count;

    new_book->next = head_node->next;
    head_node->next = new_book;

    printf("《%s》添加成功！\n", book_name);

    // 文件操作部分
    FILE *book_file = fopen("baiyun_library_book.txt", "a+");
    if (book_file == NULL)
    {
        perror("baiyun_library_book.txt open error");
        return new_book; // 即使文件写入失败，图书仍添加到链表
    }
    printf("baiyun_library_book.txt open success\n");

    char book_info[256] = {0};
    // 拼接图书信息
    sprintf(book_info, "书名:《%s》 作者:%s 出版社:%s 发行年份:%ld 数量:%d\r\n",
            new_book->book_title, new_book->author, new_book->publishing_house,
            new_book->year, new_book->amount);

    // 按字符串实际长度写入
    int info_length = strlen(book_info);
    size_t written_length = fwrite(book_info, sizeof(char), info_length, book_file);
    if (written_length != info_length)
    {
        perror("fwrite error: 写入长度不匹配");
    }

    // 刷新缓冲区，确保内容立即写入文件
    fflush(book_file);
    fclose(book_file);

    return new_book;
}

Library_Book *search_title(Library_Book *head_node)
{
    if (!head_node)
        return NULL;

    int choice = get_valid_int_input("查询方式: 1-按书名 2-按作者 3-退出: ", 1, 3);
    if (choice == 3)
        return NULL;

    char keyword[MAX_LENGTH];
    clear_input_buffer();

    if (choice == 1)
    {
        printf("请输入书名: ");
        get_string_input("", keyword, MAX_LENGTH);
    }
    else
    {
        printf("请输入作者: ");
        get_string_input("", keyword, MAX_LENGTH);
    }

    Library_Book *tmp = head_node->next;
    Library_Book *found = NULL;
    int count = 0;

    while (tmp)
    {
        int match = 0;
        if (choice == 1 && strcmp(tmp->book_title, keyword) == 0)
            match = 1;
        if (choice == 2 && strcmp(tmp->author, keyword) == 0)
            match = 1;

        if (match)
        {
            printf("\n=== 找到图书 ===\n");
            printf("书名: 《%s》\n", tmp->book_title);
            printf("作者: %s\n", tmp->author);
            printf("出版社: %s\n", tmp->publishing_house);
            printf("年份: %ld\n", tmp->year);
            printf("数量: %d\n", tmp->amount);
            printf("================\n");
            count++;
            if (!found)
                found = tmp;
        }
        tmp = tmp->next;
    }

    if (count == 0)
    {
        printf("未找到相关图书！\n");
    }
    return found;
}

User_Account *borrow_book(Library_Book *head_node, User_Account *login_user)
{
    if (!login_user)
    {
        printf("请先登录！\n");
        return NULL;
    }

    if (login_user->limit >= BORROW_LIMIT)
    {
        printf("借书数量已达上限！\n");
        return login_user;
    }

    Library_Book *book = search_title(head_node);
    if (!book)
        return login_user;

    if (book->amount <= 0)
    {
        printf("该书暂无库存！\n");
        return login_user;
    }

    printf("确认借阅《%s》？1-是 2-否: ", book->book_title);
    int confirm = get_valid_int_input("", 1, 2);
    if (confirm == 2)
        return login_user;

    strcpy(login_user->borrowed_books[login_user->limit], book->book_title);
    login_user->limit++;
    book->amount--;

    printf("借书成功！当前已借%d本\n", login_user->limit);
    return login_user;
}

Library_Book *return_book(Library_Book *head_node, User_Account *login_user)
{
    if (!login_user)
    {
        printf("请先登录！\n");
        return NULL;
    }

    if (login_user->limit == 0)
    {
        printf("您没有借书！\n");
        return NULL;
    }

    printf("您借阅的图书:\n");
    for (int i = 0; i < login_user->limit; i++)
    {
        printf("%d. 《%s》\n", i + 1, login_user->borrowed_books[i]);
    }

    char book_title[MAX_LENGTH];
    clear_input_buffer();
    printf("请输入要归还的书名: ");
    get_string_input("", book_title, MAX_LENGTH);

    int found_index = -1;
    for (int i = 0; i < login_user->limit; i++)
    {
        if (strcmp(login_user->borrowed_books[i], book_title) == 0)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("您未借阅该书！\n");
        return NULL;
    }

    // 更新借阅记录
    for (int i = found_index; i < login_user->limit - 1; i++)
    {
        strcpy(login_user->borrowed_books[i], login_user->borrowed_books[i + 1]);
    }
    strcpy(login_user->borrowed_books[login_user->limit - 1], "");
    login_user->limit--;

    // 更新图书库存
    Library_Book *tmp = head_node->next;
    Library_Book *returned_book = NULL;
    while (tmp)
    {
        if (strcmp(tmp->book_title, book_title) == 0)
        {
            tmp->amount++;
            returned_book = tmp;
            break;
        }
        tmp = tmp->next;
    }

    printf("还书成功！\n");
    return returned_book;
}

void printf_all_title(Library_Book *head_node)
{
    if (!head_node || !head_node->next)
    {
        printf("暂无图书数据！\n");
        return;
    }

    Library_Book *tmp = head_node->next;
    int count = 1;

    printf("\n=== 所有图书列表 ===\n");
    while (tmp)
    {
        printf("%d. 《%s》 - %s (%s, %ld年) 库存: %d\n",
               count, tmp->book_title, tmp->author,
               tmp->publishing_house, tmp->year, tmp->amount);
        tmp = tmp->next;
        count++;
    }
}

void borrowing_History(User_Account *login)
{
    if (!login)
    {
        printf("请先登录！\n");
        return;
    }

    if (login->limit == 0)
    {
        printf("您没有借阅记录！\n");
        return;
    }

    printf("\n=== 您的借阅记录 ===\n");
    for (int i = 0; i < login->limit; i++)
    {
        printf("%d. 《%s》\n", i + 1, login->borrowed_books[i]);
    }
}

Library_Book *delete_book_Node(Library_Book *head_node)
{
    if (!head_node || !head_node->next)
    {
        printf("暂无图书数据！\n");
        return head_node;
    }

    char title[MAX_LENGTH];
    clear_input_buffer();
    printf("请输入要删除的书名: ");
    get_string_input("", title, MAX_LENGTH);

    Library_Book *prev = head_node;
    Library_Book *curr = head_node->next;
    int found = 0;

    while (curr)
    {
        if (strcmp(curr->book_title, title) == 0)
        {
            prev->next = curr->next;
            free(curr);
            found = 1;
            printf("删除成功！\n");
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found)
        printf("未找到该书！\n");
    return head_node;
}

Library_Book *destory_book_Node(Library_Book *head_node)
{
    if (!head_node)
        return NULL;

    Library_Book *tmp = head_node->next;
    while (tmp)
    {
        Library_Book *next = tmp->next;
        free(tmp);
        tmp = next;
    }
    head_node->next = NULL;
    printf("图书系统已清空！\n");
    return head_node;
}
