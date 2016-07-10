#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
static int gName=0;
int count=10,a[2][2],attr=2;
struct table
{
	char hown[4];
	char marital[10];
	long income;
	char class[3];
}t[10],t1[10],t2[10];
typedef struct table Table;

int gini(struct table []);

struct tree
{
	int root;
	struct tree *llink,*rlink;
};
typedef struct tree *Tree;

Tree getTree()
{
	Tree tr;
	tr=(struct tree*)malloc(sizeof(struct tree));
	tr->rlink=NULL;
	tr->llink=NULL;
	return tr;
}
Tree trr;

//used to split when direct class label is not got
//t1 will have all the records to the left side and t2 to the right
void copy(Table t[],int gno)
{
	int i,j=0,k=0;
	switch(gno)
	{
		case 1:for(i=0;i<count;i++)
			{
				if(strcmp(t[i].hown,"no")!=0)
					t1[j++]=t[i];
				if(strcmp(t[i].hown,"yes")!=0)
					t2[k++]=t[i];
			}
			return;
		case 2:	j=k=0;
			for(i=0;i<count;i++)
			{
				if(strcmp(t[i].marital,"married")!=0)
					t1[j++]=t[i];
				if(strcmp(t[i].marital,"unmarried")!=0)
					t2[k++]=t[i];
			}
			return;
		case 3:j=k=0;
			for(i=0;i<count;i++)
			{
				if(t[i].income<80000)
					t1[j++]=t[i];
				if(t[i].income>=80000)
					t2[k++]=t[i];
			}
			return;
	}
}

//tree construction
//roots are given the attribute number and leaves -1 i.e,no or 0 i.e, yes
Tree construct(Tree tr,Table t[])
{
	int i,j,gno,flag=1;
	//if records are empty or then all the attributes have been used
	if(count==0 || attr==0)
		return tr;
	else
	{	
		
		gno=gini(t);
		//based on the minimum gini we get the attribute no in gno
		switch(gno)
		{
			case 1://yes is given as left child nad no as right
				tr->root=1;
				//a stores the matrix which is used while calcuating gini
				//based on attribute no it will hae the columns and rows are yes and no
				if(a[0][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=-1;
					for(i=0;i<count;i++)
						//deletes the records which have been given the class
						if((strcmp(t[i].hown,"yes")==0) && (strcmp(t[i].class,"no")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					tr->rlink=getTree();	
					construct(tr->rlink,t);
				}
				else if( a[1][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=0;
					for(i=0;i<count;i++)
						if((strcmp(t[i].hown,"yes")==0) && (strcmp(t[i].class,"yes")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->llink,t);
				}
				break;
			case 2://unmarried as left child and married as right
				tr->root=2;
				if(a[0][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=-1;
					for(i=0;i<count;i++)
						if((strcmp(t[i].marital,"unmarried")==0) && (strcmp(t[i].class,"no")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->llink,t);
				}
				else if( a[1][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=0;
					for(i=0;i<count;i++)
						if((strcmp(t[i].marital,"unmarried")==0) && (strcmp(t[i].class,"yes")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->llink,t);
				}

				else if(a[0][1]==0)
				{
					flag=0;
					tr->rlink=getTree();
					tr->rlink->root=-1;
					for(i=0;i<count;i++)
						if((strcmp(t[i].marital,"married")==0) && (strcmp(t[i].class,"no")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					tr->llink=getTree();
					construct(tr->llink,t);
				}
				else if( a[1][1]==0)
				{
					flag=0;
					tr->rlink=getTree();
					tr->llink->root=0;
					for(i=0;i<count;i++)
						if((strcmp(t[i].marital,"married")==0) && (strcmp(t[i].class,"yes")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->rlink,t);
				}
				break;
			case 3://less than is given as left child and greater than or equal to as right
				tr->root=3;
				if(a[0][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=-1;
					for(i=0;i<count;i++)
						if((t[j].income<80000) && (strcmp(t[i].class,"no")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}	
					tr->rlink=getTree();
					construct(tr->rlink,t);
				}
				else if( a[1][0]==0)
				{
					flag=0;
					tr->llink=getTree();
					tr->llink->root=0;
					for(i=0;i<count;i++)
						if((t[j].income<80000) && (strcmp(t[i].class,"yes")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->llink,t);
				}
				else if(a[0][1]==0)
				{
					flag=0;
					tr->rlink=getTree();
					tr->llink->root=-1;
					for(i=0;i<count;i++)
						if((t[j].income>=80000) && (strcmp(t[i].class,"no")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}	
					construct(tr->rlink,t);
				}
				else if( a[1][1]==0)
				{
					flag=0;
					tr->rlink=getTree();
					tr->llink->root=0;
					for(i=0;i<count;i++)
						if((t[j].income>=80000) && (strcmp(t[i].class,"yes")==0))
						{
							for(j=i;j<count;j++)
								t[j]=t[j+1];
							count--;
						}
					construct(tr->rlink,t);
				}
				break;
			}
			//if direct class cannot be assigned
				if(flag==1)
				{
					copy(t,gno);
					construct(tr->llink,t1);
					copy(t,gno);
					construct(tr->rlink,t2);
				}
	}
return trr;
}

//formula for gini calculation
float formula()
{
	float g1,g,g2;
	g1=(float)pow((float)a[0][0]/(a[0][0]+a[1][0]),2)+(float)pow((float)a[1][0]/(a[0][0]+a[1][0]),2);
	g2=(float)pow((float)a[0][1]/(a[0][1]+a[1][1]),2)+(float)pow((float)a[1][1]/(a[0][1]+a[1][1]),2);
	g=(1-g1)*((a[0][0]+(float)a[1][0])/count)+(1-g2)*((float)(a[0][1]+a[1][1])/count);
	return g;
}

//finding gini for all attributes and depending on which has minimum gini attribute numb is returned
int gini(Table t[])
{
	int i,j,pos;
	int yyno=0,ynno=0,nyno=0,nnno=0,syno=0,myno=0,dyno=0,snno=0,mnno=0,dnno=0,lyno=0,lnno=0,gyno=0,gnno=0;
	int gno,b[2][3],a1,a2,a3,a4;
	float g1,g2,g;
	//3 columns
	for(i=1;i<4;i++)
	{
		switch(i)
		{
			case 1:
				for(j=0;j<count;j++)
				{
					if(strcmp(t[j].hown,"yes")==0)
					{
						if(strcmp(t[j].class,"yes")==0)
							yyno++;
						else
							ynno++;
						a[0][0]=yyno;
						a[1][0]=ynno;
						
					}
					else
					{
						if(strcmp(t[j].class,"yes")==0)
							nyno++;
						else
							nnno++;
					
						a[0][1]=nyno;
						a[1][1]=nnno;
					}	
				}
				g=formula();
				pos=1;
				break;
			case 2:for(j=0;j<count;j++)
				{
					if(strcmp(t[j].marital,"unmarried")==0)
					{
						if(strcmp(t[j].class,"yes")==0)
							syno++;
						else
							snno++;
						a[0][0]=syno;
						a[1][0]=snno;
					}
					else if(strcmp(t[j].marital,"married")==0)
					{
						if(strcmp(t[j].class,"yes")==0)
							myno++;
						else
							mnno++;
						a[0][1]=myno;
						a[1][1]=mnno;
					}
				}
				g1=formula();
				if(g>g1)
				{
					g=g1;
					pos=2;
					attr--;
				}			
				break;
			case 3:
				for(j=0;j<count;j++)
				{
					if(t[j].income<80000)
					{
						if(strcmp(t[j].class,"yes")==0)
							lyno++;
						else
							lnno++;
					}
					else
					{
						if(strcmp(t[j].class,"yes")==0)
							gyno++;
						else
							gnno++;
					}
					a[0][0]=lyno;
					a[1][0]=lnno;
					a[0][1]=gyno;
					a[1][1]=gnno;
				}
				g1=formula();
				if(g>g1)
				{
					g=g1;
					pos=3;
					attr--;
				}
				break;				
			}
	}
	switch(pos)
	{
		case 1:a1=yyno;
			a2=nyno;
			a3=ynno;
			a4=nnno;
			break;
		case 2:
			a1=syno;
			a2=myno;
			a3=snno;
			a4=mnno;
			break;
		case 3:
			a1=lyno;
			a2=gyno;
			a3=lnno;
			a4=gnno;
			break;
	}
	a[0][0]=a1;
	a[0][1]=a2;
	a[1][0]=a3;
	a[1][1]=a4;
	return pos;
}

//to take input
Table input()
{
	Table ta;
	scanf("%s",ta.hown);
	scanf("%s",ta.marital);
	scanf("%ld",&ta.income);
	scanf("%s",ta.class);
	return ta;
}

void display(Tree tr)
{
	if(tr==NULL)
		return;
	else
	{
		printf("%d\n",tr->root);
		display(tr->llink);
		display(tr->rlink);
	}
}
int test(Table ta,Tree tr)
{
	int i,attr,flag=1;
	if(tr->rlink==NULL && tr->llink==NULL)
	{
		if(tr->root==0)
			if(strcmp(ta.class,"yes")!=0)
				return 0;
			else
				return 1;
		else if(tr->root==-1)
			if(strcmp(ta.class,"no")!=0)
				return 0;
			else
				return 1;
	}
	else
	{
		attr=tr->root;
		switch(attr)
		{
			case 1:if(strcmp(ta.hown,"yes")==0)
					test(ta,tr->llink);
				else
					test(ta,tr->rlink);
				break;
			case 2:if(strcmp(ta.marital,"unmarried")==0)
					test(ta,tr->llink);
				else
					test(ta,tr->rlink);
				break;
			case 3:if(ta.income<80000)
					test(ta,tr->llink);
				else
					test(ta,tr->rlink);
				break;
		}
	}
}
int c=4;
//dotty
void preorderDotDump (Tree r, FILE* outputFile)
{
	if (r->llink!=NULL && r->rlink!=NULL) 
	{	fprintf (outputFile, "%d [label=%d,color=black];\n",r->root, r->root);
		if (r->llink != NULL) 
			if(r->llink->root==-1)
			{      	fprintf (outputFile, "%d -> %d;\n", r->root,c);
				fprintf (outputFile, "%d [label=\"no\"];\n",c);
				c++;
			}
			else
				fprintf (outputFile, "%d -> %d;\n", r->root,  (r->llink)->root);
		if (r->rlink != NULL)
			if(r->rlink->root==-1)
            			{      	fprintf (outputFile, "%d -> %d;\n", r->root,c);
				fprintf (outputFile, "%d [label=\"no\"];\n",c);c++;	}
			else if(r->rlink->root==0)
				{      	fprintf (outputFile, "%d -> %d;\n", r->root,c);
				fprintf (outputFile, "%d [label=\"yes\"];\n",c);c++;	}
			else
				fprintf (outputFile, "%d -> %d;\n", r->root,  (r->rlink)->root);
		c++;
        	preorderDotDump (r->llink, outputFile);
        	preorderDotDump (r->rlink, outputFile);
    	}
}

void dotDump(Tree r, FILE *outFile)
{
	gName++;
	fprintf (outFile, "digraph{\ngraph[ordering=\"out\"];\n",gName);
	
	preorderDotDump (r, outFile);
    	fprintf (outFile, "}\n");
}
	
FILE *outputFile;

int main()
{                                       
	FILE *pipe;
	int i,flag,cnt;
	Tree tr;
	for (i=0;i<20;i++)
	{
		t[i]=input();
	}
	tr=getTree();
	trr=getTree();
	trr=tr;
	tr=construct(tr,t);
	display(tr);
	for(i=10;i<20;i++)
	{
		flag=0;
		flag=test(t[i],tr);
		if(flag==1)
			{
			printf("Matching\n");cnt++;}
		else
			printf("Not Matching\n");
	}
	outputFile = fopen ("bst.dot", "w");		
	outputFile = fopen ("bst.dot", "a");
			if (outputFile != NULL) 
			{
				dotDump (trr,  outputFile);
			}
			fclose (outputFile);	
	pipe=popen("dot -Tps bst.dot -o bst.ps","w");
					pclose(pipe);	
					pipe=popen("evince bst.ps","r"); 
					pclose(pipe);
}	
