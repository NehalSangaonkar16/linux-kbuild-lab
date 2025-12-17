#include <linux/module.h>
#include "helper.h"

static int __init kbuild_lab_init(void)
{
    pr_info("kbuild_lab: init called\n");

#ifdef CONFIG_KBUILD_LAB_DEBUG
    pr_info("kbuild_lab: debug enabled\n");
#endif

    helper_function();
    return 0;
}

static void __exit kbuild_lab_exit(void)
{
    pr_info("kbuild_lab: exit called\n");
}

module_init(kbuild_lab_init);
module_exit(kbuild_lab_exit);

MODULE_LICENSE("GPL");
