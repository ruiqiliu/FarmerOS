commit 824a855e0b7c95fda908f514b8782efaebe23b50
Author: tracer <tracer@njuoslab.org>
Date:   Mon May 12 17:15:32 2014 +0800

    > play

diff --git a/include/common.h b/include/common.h
index f3f8b1d..cd9509c 100644
--- a/include/common.h
+++ b/include/common.h
@@ -4,5 +4,6 @@
 #include "common/types.h"
 #include "common/const.h"
 #include "common/assert.h"
+#include "common/list.h"
 
 #endif
diff --git a/include/common/list.h b/include/common/list.h
new file mode 100644
index 0000000..896ca89
--- /dev/null
+++ b/include/common/list.h
@@ -0,0 +1,248 @@
+#ifndef __LIST_H
+#define __LIST_H
+//copied from http://isis.poly.edu/kulesh/stuff/src/klist/list.h
+//change identifier new to new_node
+// added by ridox
+
+
+/* This file is from Linux Kernel (include/linux/list.h)
+ * and modified by simply removing hardware prefetching of list items.
+ * Here by copyright, credits attributed to wherever they belong.
+ * Kulesh Shanmugasundaram (kulesh [squiggly] isis.poly.edu)
+ */
+
+/*
+ * Simple doubly linked list implementation.
+ *
+ * Some of the internal functions ("__xxx") are useful when
+ * manipulating whole lists rather than single entries, as
+ * sometimes we already know the next/prev entries and we can
+ * generate better code by using them directly rather than
+ * using the generic single-entry routines.
+ */
+
+struct list_head {
+	struct list_head *next, *prev;
+};
+
+#define LIST_HEAD_INIT(name) { &(name), &(name) }
+
+#define LIST_HEAD(name) \
+	struct list_head name = LIST_HEAD_INIT(name)
+
+#define INIT_LIST_HEAD(ptr) do { \
+	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
+} while (0)
+
+/*
+ * Insert a new entry between two known consecutive entries.
+ *
+ * This is only for internal list manipulation where we know
+ * the prev/next entries already!
+ */
+static inline void __list_add(struct list_head *new_node,
+			      struct list_head *prev,
+			      struct list_head *next)
+{
+	next->prev = new_node;
+	new_node->next = next;
+	new_node->prev = prev;
+	prev->next = new_node;
+}
+
+/**
+ * list_add - add a new entry
+ * @new: new entry to be added
+ * @head: list head to add it after
+ *
+ * Insert a new entry after the specified head.
+ * This is good for implementing stacks.
+ */
+static inline void list_add(struct list_head *new_node, struct list_head *head)
+{
+	__list_add(new_node, head, head->next);
+}
+
+/**
+ * list_add_tail - add a new entry
+ * @new: new entry to be added
+ * @head: list head to add it before
+ *
+ * Insert a new entry before the specified head.
+ * This is useful for implementing queues.
+ */
+static inline void list_add_tail(struct list_head *new_node, struct list_head *head)
+{
+	__list_add(new_node, head->prev, head);
+}
+
+/*
+ * Delete a list entry by making the prev/next entries
+ * point to each other.
+ *
+ * This is only for internal list manipulation where we know
+ * the prev/next entries already!
+ */
+static inline void __list_del(struct list_head *prev, struct list_head *next)
+{
+	next->prev = prev;
+	prev->next = next;
+}
+
+/**
+ * list_del - deletes entry from list.
+ * @entry: the element to delete from the list.
+ * Note: list_empty on entry does not return true after this, the entry is in an undefined state.
+ */
+static inline void list_del(struct list_head *entry)
+{
+	__list_del(entry->prev, entry->next);
+	entry->next = (void *) 0;
+	entry->prev = (void *) 0;
+}
+
+/**
+ * list_del_init - deletes entry from list and reinitialize it.
+ * @entry: the element to delete from the list.
+ */
+static inline void list_del_init(struct list_head *entry)
+{
+	__list_del(entry->prev, entry->next);
+	INIT_LIST_HEAD(entry);
+}
+
+/**
+ * list_move - delete from one list and add as another's head
+ * @list: the entry to move
+ * @head: the head that will precede our entry
+ */
+static inline void list_move(struct list_head *list, struct list_head *head)
+{
+        __list_del(list->prev, list->next);
+        list_add(list, head);
+}
+
+/**
+ * list_move_tail - delete from one list and add as another's tail
+ * @list: the entry to move
+ * @head: the head that will follow our entry
+ */
+static inline void list_move_tail(struct list_head *list,
+				  struct list_head *head)
+{
+        __list_del(list->prev, list->next);
+        list_add_tail(list, head);
+}
+
+/**
+ * list_empty - tests whether a list is empty
+ * @head: the list to test.
+ */
+static inline int list_empty(struct list_head *head)
+{
+	return head->next == head;
+}
+
+static inline void __list_splice(struct list_head *list,
+				 struct list_head *head)
+{
+	struct list_head *first = list->next;
+	struct list_head *last = list->prev;
+	struct list_head *at = head->next;
+
+	first->prev = head;
+	head->next = first;
+
+	last->next = at;
+	at->prev = last;
+}
+
+/**
+ * list_splice - join two lists
+ * @list: the new list to add.
+ * @head: the place to add it in the first list.
+ */
+static inline void list_splice(struct list_head *list, struct list_head *head)
+{
+	if (!list_empty(list))
+		__list_splice(list, head);
+}
+
+/**
+ * list_splice_init - join two lists and reinitialise the emptied list.
+ * @list: the new list to add.
+ * @head: the place to add it in the first list.
+ *
+ * The list at @list is reinitialised
+ */
+static inline void list_splice_init(struct list_head *list,
+				    struct list_head *head)
+{
+	if (!list_empty(list)) {
+		__list_splice(list, head);
+		INIT_LIST_HEAD(list);
+	}
+}
+
+/**
+ * list_entry - get the struct for this entry
+ * @ptr:	the &struct list_head pointer.
+ * @type:	the type of the struct this is embedded in.
+ * @member:	the name of the list_struct within the struct.
+ */
+#define list_entry(ptr, type, member) \
+	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
+
+/**
+ * list_for_each	-	iterate over a list
+ * @pos:	the &struct list_head to use as a loop counter.
+ * @head:	the head for your list.
+ */
+#define list_for_each(pos, head) \
+	for (pos = (head)->next; pos != (head); \
+        	pos = pos->next)
+/**
+ * list_for_each_prev	-	iterate over a list backwards
+ * @pos:	the &struct list_head to use as a loop counter.
+ * @head:	the head for your list.
+ */
+#define list_for_each_prev(pos, head) \
+	for (pos = (head)->prev; pos != (head); \
+        	pos = pos->prev)
+
+/**
+ * list_for_each_safe	-	iterate over a list safe against removal of list entry
+ * @pos:	the &struct list_head to use as a loop counter.
+ * @n:		another &struct list_head to use as temporary storage
+ * @head:	the head for your list.
+ */
+#define list_for_each_safe(pos, n, head) \
+	for (pos = (head)->next, n = pos->next; pos != (head); \
+		pos = n, n = pos->next)
+
+/**
+ * list_for_each_entry	-	iterate over list of given type
+ * @pos:	the type * to use as a loop counter.
+ * @head:	the head for your list.
+ * @member:	the name of the list_struct within the struct.
+ */
+#define list_for_each_entry(pos, head, member)				\
+	for (pos = list_entry((head)->next, typeof(*pos), member);	\
+	     &pos->member != (head); 					\
+	     pos = list_entry(pos->member.next, typeof(*pos), member))
+
+/**
+ * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
+ * @pos:	the type * to use as a loop counter.
+ * @n:		another type * to use as temporary storage
+ * @head:	the head for your list.
+ * @member:	the name of the list_struct within the struct.
+ */
+#define list_for_each_entry_safe(pos, n, head, member)			\
+	for (pos = list_entry((head)->next, typeof(*pos), member),	\
+		n = list_entry(pos->member.next, typeof(*pos), member);	\
+	     &pos->member != (head); 					\
+	     pos = n, n = list_entry(n->member.next, typeof(*n), member))
+
+
+#endif
diff --git a/include/thread.h b/include/thread.h
new file mode 100644
index 0000000..888b706
--- /dev/null
+++ b/include/thread.h
@@ -0,0 +1,6 @@
+#ifndef __THREAD_H__
+#define __THREAD_H__
+
+#include "thread/pcb.h"
+
+#endif
diff --git a/include/thread/pcb.h b/include/thread/pcb.h
new file mode 100644
index 0000000..8662731
--- /dev/null
+++ b/include/thread/pcb.h
@@ -0,0 +1,22 @@
+#ifndef PCB_H
+#define PCB_H
+
+#define STK_SZ 1024
+struct Thread{
+	TrapFrame *tf;
+
+	list_head runq, freeq;
+
+	char kstack[STK_SZ];
+};
+
+// 创建一个内核线程
+Thread *create_kthread(void (*entry)(void));
+// 使当前进程/线程立即阻塞，并可以在未来被唤醒
+void sleep(void);
+// 唤醒一个进程/线程
+void wakeup(Thread *t);
+// 短临界区保护，实现关中断保护的原子操作
+void lock(void);
+void unlock(void);
+#endif
diff --git a/include/x86/memory.h b/include/x86/memory.h
index 327f812..1256781 100644
--- a/include/x86/memory.h
+++ b/include/x86/memory.h
@@ -19,9 +19,14 @@ struct GateDescriptor {
 	uint32_t offset_31_16     : 16;
 };
 
+//Interrupt and exception frame
+// http://cslab.nju.edu.cn/opsystem/#OS2014_5
 struct TrapFrame {
-	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;
+	uint32_t edi, esi, ebp, esp_, xxx, ebx, edx, ecx, eax;// no esp
+	uint32_t gs, fs, es, ds;
 	int32_t irq;
+	uint32_t err, eip, cs, eflags;
 };
+typedef struct TrapFrame TrapFrame;
 
 #endif
diff --git a/src/irq/irq_handle.c b/src/irq/irq_handle.c
index fc31883..a1ac5ad 100644
--- a/src/irq/irq_handle.c
+++ b/src/irq/irq_handle.c
@@ -3,6 +3,17 @@
 
 void
 irq_handle(struct TrapFrame *tf) {
+	if(tf->irq < 1000){
+		if(tf->irq == -1){
+			printf("%s, %d: Unhandled exception!\n",__FUNCTION__,__LINE__);
+		}
+		else{
+			printf("%s, %d: Unexpected exception %d!\n",__FUNCTION__,__LINE__, tf->irq);
+		}
+		assert(0);
+	}
+
+
 	if (tf->irq == 1000) {
 		printf(".");
 	} else if (tf->irq == 1001) {
@@ -10,7 +21,9 @@ irq_handle(struct TrapFrame *tf) {
 		uint32_t val = in_byte(0x61);
 		out_byte(0x61, val | 0x80);
 		out_byte(0x61, val);
-		printf("code is : %d\n",code);
+		printf("%s, %d:key code = %x\n",__FUNCTION__,__LINE__, code);
+
+		//do_keyboard(code);
 	} else {
 		assert(0);
 	}
