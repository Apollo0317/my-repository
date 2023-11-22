#include<stdio.h>
#include<stdlib.h>

struct link {
    int num;
    struct link* next;
};

int main(void) {
    struct link* head = (struct link*)malloc(sizeof(struct link));
    struct link* p = head;
    int n = 0;
    
    printf("input the length of the linklist plz\n");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        struct link* s = (struct link*)malloc(sizeof(struct link));
        scanf("%d", &s->num);
        p->next = s;
        s->next = NULL;
        p = s;
    }  // 尾插法创建链表

    p = head->next;
    while (p != NULL) {
        printf("%d ", p->num);
        p = p->next;
    }
    printf("\n");

    int x = 0, po = 0, count = 0;
    
    printf("input the insert number and its position (block number) plz\n");
    
    struct link* insert = (struct link*)malloc(sizeof(struct link));
    struct link* preinsert = head;
    struct link* afterinsert = head->next;

    scanf("%d", &insert->num);
    scanf("%d", &po);

	while(afterinsert!=NULL){
	if(count==po){
		preinsert->next=insert;
		insert->next=afterinsert;
		break;
	}
	else{
		count++;
		preinsert=afterinsert;
		if(afterinsert!=NULL){
			afterinsert=afterinsert->next;
		}
	}

	}

    p = head->next;
    while (p != NULL) {
        printf("%d ", p->num);
        p = p->next;
    }

    return 0;
}
