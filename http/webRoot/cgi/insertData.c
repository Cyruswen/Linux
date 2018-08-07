#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void insert_data(char* name, char* sex, char* phone)
{
    MYSQL* mysql_fd = mysql_init(NULL);
    if(mysql_real_connect(mysql_fd, "127.0.0.1", "root","123456", "student", 3306, NULL, 0 ) == NULL)
    {
        printf("connect failed!\n");
        return;
    }
    printf("connect mysql success!\n");

    char sql[1024];
    sprintf(sql, "insert into student (name, sex, phone) values (\"%s\", \"%s\", \"%s\")", name, sex, phone);
    printf("sql: %s\n", sql);
    mysql_query(mysql_fd, sql);
    mysql_close(mysql_fd);
}

int main()
{
    char data[1024];
    if(getenv("METHOD")){
        if(strcasecmp("GET", getenv("METHOD")) == 0)
        {
            strcpy(data, getenv("QUERY_STRING"));
        }else{
            int content_length = atoi(getenv("CONTENT_LENGTH"));
            int i = 0;
            for(; i < content_length; i++)
            {
                read(0, data+i,1);
            }
            data[i] = 0;
        }
    }
    printf("arg: %s\n", data);
    char* name;
    char* sex;
    char* phone;
    
    strtok(data, "=&");
    name = strtok(NULL, "=&");
    strtok(NULL, "=&");
    sex = strtok(NULL, "=&");
    strtok(NULL, "=&");
    phone = strtok(NULL, "=&");
    insert_data(name, sex, phone);
    return 0;
}
