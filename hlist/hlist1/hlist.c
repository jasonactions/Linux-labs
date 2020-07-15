#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

/*定义哈希桶的数量*/
#define  CMD_HASH_HEAD_SIZE        27

typedef struct cmd_hash_head {
	struct	hlist_head head; /*哈希桶的首地址*/
	
	int8_t	ch;	/* 哈希桶的关键字(a~z,?，总共27个)*/    
	int8_t	offset; /* 这个哈希桶在整个哈希表中的偏移*/
	int16_t	count;	/* 当前哈希桶中节点的个数*/
} cmd_hash_head_t;


typedef struct cmd_hash_node {
	struct hlist_node node;
	int8_t		name[20];
} cmd_hash_node_t;


/*定义CMD_HASH_HEAD_SIZE大小的哈希桶*/
static cmd_hash_head_t	cmd_hash[CMD_HASH_HEAD_SIZE];
cmd_hash_node_t *hash_node;

/*对哈希桶进行初始化*/
static void cmd_hash_init(void)
{
	int32_t index = 0;
	
	memset(cmd_hash, 0, sizeof(cmd_hash));
	
	for (index = 0; index < (CMD_HASH_HEAD_SIZE - 1); index++) {
		INIT_HLIST_HEAD(&cmd_hash[index].head);
		cmd_hash[index].count	= 0;
		cmd_hash[index].offset	= index;
		cmd_hash[index].ch	= 'a' + index;
	}    
	
	index = CMD_HASH_HEAD_SIZE - 1;
	INIT_HLIST_HEAD(&cmd_hash[index].head);
	cmd_hash[index].count	= 0;
	cmd_hash[index].offset	= index;
	cmd_hash[index].ch		= '?';
}


/*散列表*/
static int8_t node_type(int8_t *name)
{
    int8_t    ch         = 0x00;
    int8_t    offset     = 0;
    if (name == NULL)    return -1;

    ch    = name[0];
    switch(ch)
    {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            offset = ch - 'a' + 0;
            break;

        default :
            offset = 26;
            break;
    }

    return offset;    
}

static void hash_node_init(int8_t *name)
{
	int8_t        offset = 0;
	
	hash_node = kzalloc(sizeof(cmd_hash_node_t), GFP_KERNEL); 
	
	offset = node_type(name);
	if(offset < 0)    return;
	
	strlcpy(hash_node->name, name, strlen((char*)name));
	cmd_hash[offset].count++;
	
	INIT_HLIST_NODE(&hash_node->node);
	hlist_add_head(&hash_node->node, &cmd_hash[offset].head);
}


static void cmd_hash_node_init(void)
{
	hash_node_init((int8_t*)"hello");
	hash_node_init((int8_t*)"help");
	hash_node_init((int8_t*)"scan");
	hash_node_init((int8_t*)"???");
}

static void cmd_hash_node_show(void)
{
	int32_t index = 0;
	int16_t count = 0;
	
	cmd_hash_node_t *cmd_hnode = NULL;
	
	printk("=============current bucket\n");
	
	for (index = 0; index < CMD_HASH_HEAD_SIZE; index++) {
		count = cmd_hash[index].count;
		if (count <= 0)
			continue;

		printk("hash%d, count : %d,offset : %d, ch : %c\n",
			index,
			cmd_hash[index].count,
			cmd_hash[index].offset,
			cmd_hash[index].ch);
			
		hlist_for_each_entry(cmd_hnode, &cmd_hash[index].head, node)
			printk("name : %s\n", cmd_hnode->name);
		printk("\n");
	}
}

static void test_hlist_oper(void)
{
	cmd_hash_init();
	cmd_hash_node_init();
	cmd_hash_node_show();    
}


static int __init hlist_init(void)
{
	test_hlist_oper();

	return 0;
}

static void __exit hlist_exit(void)
{
	kfree(hash_node);

}

module_init(hlist_init);
module_exit(hlist_exit);

MODULE_LICENSE("GPLv2");
MODULE_AUTHOR("HZero <jasonactions@foxmail.com>");
MODULE_DESCRIPTION("hlist demo");
