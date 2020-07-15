/*
 * https://blog.csdn.net/wjlkoorey/article/details/52012998
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h> 

#define MAX_STRING_LEN 32
#define MAX_HLIST_COUNT 2 //只有“男”、“女”两条哈希链表

typedef struct student
{
        char m_name[MAX_STRING_LEN];
        char m_sex;
        int m_age;
        struct list_head m_list; /*把我们的学生对象组织成双向链表，就靠该节点了*/
        struct hlist_node m_hlist; /*把我们的学生对象组织成哈希链表，就靠该节点了*/
}Student;


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Koorey Wung");

static int dbg_flg = 0;

LIST_HEAD(g_student_list);

// 其中g_stu_hlist[0]代表男生；g_stu_hlist[1]代表女生
struct hlist_head g_stu_hlist[MAX_HLIST_COUNT];

//初始化男、女学生的哈希链表
static void init_hlists(void)
{
    int i = 0;
    for(i=0;i< MAX_HLIST_COUNT;i++){
        INIT_HLIST_HEAD(&g_stu_hlist[i]);
    }
}

static int add_stu(char* name,char sex,int age)
{
    Student *stu,*cur_stu;

    list_for_each_entry(cur_stu, &g_student_list, m_list){ //仅遍历是否有同名学生，所以用该接口
        if(!strcmp(cur_stu->m_name, name))
        {
            printk("Error:the name confict!\n");
            return -1;
        }
    }

    stu = kmalloc(sizeof(Student), GFP_KERNEL);
    if(!stu)
    {
        printk("kmalloc mem error!\n");
        return -1;
    }

    memset(stu,0,sizeof(Student));
    strncpy(stu->m_name, name, strlen(name));
    stu->m_sex = sex;
    stu->m_age = age;
    INIT_LIST_HEAD(&stu->m_list);    //初始化宿主结构里的双向链表节点m_list
    INIT_HLIST_NODE(&stu->m_hlist);  //初始化宿主结构里的哈希节点m_hlist

    if(dbg_flg)
        printk("(Add)name:[%s],\tsex:[%c],\tage:[%d]\n",stu->m_name,stu->m_sex,stu->m_age);


    list_add_tail(&stu->m_list,&g_student_list); //将新学生插入到链表尾部，很简单吧

    return 0;
}
EXPORT_SYMBOL(add_stu); //导出该函数，后面我们要在其他模块里调用，为了便于测试，下面其他几个接口类似

static int del_stu(char *name)
{
        Student *cur,*next;
        int ret = -1;
        list_for_each_entry_safe(cur,next,&g_student_list,m_list){ //因为要删除链表的节点，所以必须有带有“safe”的宏接口
                if(!strcmp(name,cur->m_name))
                {
                        list_del(&cur->m_list);
                        printk("(Del)name:[%s],\tsex:[%c],\tage:[%d]\n",cur->m_name,\
                                        cur->m_sex,cur->m_age);
                        kfree(cur);
                        cur = NULL;
                        ret = 0;
                        break;
                }
        }
        return ret;
}
EXPORT_SYMBOL(del_stu);

static void dump_students(void)
{
        Student *stu;
        int i = 1;
        printk("===================Student List================\n");
        list_for_each_entry(stu,&g_student_list,m_list){ //同样，也仅遍历链表而已
                printk("(%d)name:[%s],\tsex:[%c],\tage:[%d]\n",i++,stu->m_name,\
                        stu->m_sex,stu->m_age);
        }
        printk("===============================================\n");
}
EXPORT_SYMBOL(dump_students);

static void dump_hlist(int id)
{
        Student *stu;
        struct hlist_head *head;
        int count = 1;

        if(!(id>=0 && id< MAX_HLIST_COUNT)){
                printk("Invalid id[%d] !\n",id);
                return;
        }
        head = &g_stu_hlist[id];

        printk("===================%s List===================\n",((id == 0)?"Boy":"Girl"));
        //因为该接口只遍历哈希表，并不会插入、删除节点，所以用hlist_for_each_entry()，注意三个入参的类型、作用和意义
        hlist_for_each_entry(stu, head, m_hlist){
                printk("(%d)name:[%s],\tsex:[%c],\tage:[%d]\n",count++,stu->m_name,\
                        stu->m_sex,stu->m_age);
        }
        printk("==============================================\n");
}
EXPORT_SYMBOL(dump_hlist);

//分别打印男女学生，各自哈希链表上的情况
static void dump_hlists(void)
{
        dump_hlist(0);
        dump_hlist(1);
}
EXPORT_SYMBOL(dump_hlists);


//按照性别对学生进行分类
static void classify_stu(void)
{
        Student *cur,*next;
        int id = 0;

        list_for_each_entry_safe(cur, next, &g_student_list, m_list){
                //将从cur从g_student_list链表上移下来，但并不会释放cur学生的内存空间，同时对其m_list成员重新初始化
                list_del_init(&cur->m_list);
                if('m' == cur->m_sex){
                        id = 0;
                }
                else if('f' == cur->m_sex){
                        id = 1;
                }
                else{
                        printk("Get error!\n");
                        return;
                }
                //根据id，以m_hlist将学生按性别组织成哈希表
                hlist_add_head(&(cur->m_hlist),&(g_stu_hlist[id]));
        }
        printk("Finished!\n");
}
EXPORT_SYMBOL(classify_stu);


static void init_system(void)
{
    //初始化男、女学生哈希链表头
    init_hlists();

    /*系统启动初始化时，向链表g_student_list里添加6个学生*/
    add_stu("Tom",'m',18);
    add_stu("Jerry",'f',17);
    add_stu("Alex",'m',18);
    add_stu("Conory",'f',18);
    add_stu("Frank",'m',17);
    add_stu("Marry",'f',17);
}

/*释放所有哈希链表上的内存空间*/
static void clean_up_hlist(void)
{
    int i;
    Student *stu;
    struct hlist_node *n;

    for(i=0;i< MAX_HLIST_COUNT;i++){
        printk("===================%s List================\n",((i == 0)?"Boy":"Girl"));
        hlist_for_each_entry_safe(stu, n, &(g_stu_hlist[i]), m_hlist){
            hlist_del(&(stu->m_hlist));
            printk("Destroy [%s]\n",stu->m_name);
            kfree(stu);
        }
        printk("===========================================\n");
    }
}

/*释放双向表上的内存空间*/
static void clean_up_list(void)
{
        Student *stu,*next;
        printk("===========Unclassified Student List===========\n");
        list_for_each_entry_safe(stu,next,&g_student_list,m_list){
                list_del(&stu->m_list);
                printk("Destroy [%s]\n",stu->m_name);
                kfree(stu);
        }
        printk("===============================================\n");
}

/*因为没有数据库，所以当我们的模块退出时，需要释放内存。*/
static void clean_up(void)
{
        clean_up_list();
        clean_up_hlist();
}

/*模块初始化接口*/
static int student_mgt_init(void)
{
        printk("Student Managment System,Initializing...\n");

        init_system();
        dbg_flg = 1; //从此以后，再调用add_stu()时，都会有有内核打印信息，详见实例训练
        dump_students();
	classify_stu();
	dump_hlists();

        return 0;
}

static void student_mgt_exit(void)
{
        clean_up();
        printk("System Terminated!\n");
}

module_init(student_mgt_init);
module_exit(student_mgt_exit);
