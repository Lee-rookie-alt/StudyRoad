#include <my_cpp_header.h>
using namespace std;
enum Color{
    RED,
    BLACK
};
template <class K,class V>
struct RbTreeNode  //红黑树节点
{
    pair<K,V> _kv;
    RbTreeNode *_left;
    RbTreeNode *_right;
    RbTreeNode *_parent;
    Color _col;
    RbTreeNode(const pair<K,V> &kv)
    :_kv(kv)
     ,_left(nullptr)
     ,_right(nullptr)
     ,_parent(nullptr)
    {}
};
template<class K,class V>
class RbTree
{
    typedef RbTreeNode<K,V> Node;
public:
    bool insert(const pair<K,V> &kv){
        if(_root==nullptr){
            _root=new Node(kv);
            _root->_col=BLACK;
            return true;
        }
        Node *cur=_root;
        Node *parent =nullptr;
        while(cur){
            if(kv.first>cur->_kv.first){ //kv的key值大于当前节点key值往右走
                parent=cur;
                cur=cur->_right;
            }else if(kv.first<cur->_kv.first){
                parent=cur;
                cur=cur->_left;
            }else{
                return false;//代表我找到了相同key值的节点
            }
        }//这个结束后代表我查找插入的位置已完成
        cur=new Node(kv);
        cur->_col=RED;
        if(cur->_kv.first<parent->_kv.first){
            parent->_left=cur;
        }else{
            parent->_right=cur;
        }
        cur->_parent=parent;
        while(parent && parent->_col==RED){
            Node * grandfather=parent->_parent;
            if(grandfather->_left==parent)//父节点是爷节点的左孩子
            {
                Node * uncle=grandfather->_right;
                if(uncle && uncle->_col==RED)//叔节点为红色
                {
                    grandfather->_col=RED;
                    parent->_col=BLACK;
                    uncle->_col=BLACK;
                    cur=grandfather;
                    parent=cur->_parent;
                }else if((uncle && uncle->_col==BLACK) || !uncle){
                    if(cur==parent->_left)//插入节点是父节点的左节点 LL
                    {
                        RotateR(grandfather);
                        parent->_col=BLACK;
                        grandfather->_col=RED;
                    }else{//LR情况，需要先进行一次左旋再进行一次右旋
                        RotateL(parent);
                        RotateR(grandfather);
                        cur->_col=BLACK;
                        grandfather->_col=RED;
                    }
                    break;
                }
            }
            else{//父亲节点是爷节点的右孩子
                Node * uncle=grandfather->_left;
                if(uncle && uncle->_col==RED){
                    grandfather->_col=RED;
                    parent->_col=BLACK;
                    uncle->_col=BLACK;
                    cur=grandfather;
                    parent=cur->_parent;
                }else if((uncle && uncle->_col==BLACK) || !uncle){
                    if(cur==parent->_right){//RR情况，需进行一次左旋
                        RotateL(grandfather);
                        parent->_col=BLACK;
                        grandfather->_col=RED;
                    }
                    else{//RL情况，需要先进行一次右旋再进行一次左旋
                        RotateR(parent);
                        RotateL(grandfather);
                        cur->_col=BLACK;
                        grandfather->_col=RED;
                    }
                break;
                }
            }
        }
        _root->_col=BLACK;
        return true;
    }
    bool Delete(const pair<K,V> &kv){

    }
    Node * Find(const K &key){
        Node * cur=_root;
        while(cur){
            if(key>cur->_kv.first){
                cur=cur->_right;
            }else if(key<cur->_kv.first){
                cur=cur->left;
            }else{
                return cur;
            }
        }
        return nullptr;
    }
    bool Check(Node * root,int blacknum,const int refNum){
        if(root ==nullptr){
            if(blacknum!=refNum){
                cout<<"路径上有黑节点数量不相等的情况"<<endl;
                return false;
            }
            return true;
        }
        if(root->_col==RED && root->_parent->_col==RED){
            cout<<"存在两个相邻的红节点的情况"<<endl;
            return false;
        }
        if(root->_col==BLACK){
            blacknum++;
        }
        return Check(root->_left,blacknum,refNum) && Check(root->_right,blacknum,refNum);
    }
    bool isBalance(){
        if(_root == nullptr){
            return true;
        }
        if(_root->_col==RED){
            return false;
        }
        int refnum=0;
        Node *cur=_root;//先遍历一条路径的黑节点用来为之后的遍历做参考
        while(cur){
            if(cur->_col==BLACK){
                refnum++;
            }
            cur=cur->_left;
        }
        return Check(_root,0,refnum);
    }
    void InOrder(){
        _InOrder(_root);
    }
    int Size(){
        return _Size(_root);
    }
    int Height(){
        return _Height(_root);
    }
private:
   void RotateR(Node * parent){//右旋
        Node * subL=parent->_left;
        Node * subLR=subL->_right;
        Node * grandp=parent->_parent;
        parent->_left=subLR;
        subL->_right=parent;
        if(subLR)   subLR->_parent=parent;
        if(parent==_root){//parent节点没有父节点
            _root=subL;
            subL->_parent=nullptr;
        }else{
            if(parent==grandp->_left){
                grandp->_left=subL;
            }else{
                grandp->_right=subL;
            }
            subL->_parent=grandp;
        }
        parent->_parent=subL;
   }
   void RotateL(Node * parent){//左旋
        Node * subR=parent->_right;
        Node * subRL=subR->_left;
        Node *grandp =parent->_parent;
        parent->_right=subRL;
        subR->_left=parent;
        if(subRL) subRL->_parent=parent;
        if(parent==_root){
            _root=subR;
            subR->_parent=nullptr;
        }else{
            if(parent==grandp->_left){
                grandp->_left=subR;
            }else{
                grandp->_right=subR;
            }
            subR->_parent=grandp;
        }
        parent->_parent=subR;
   }
   int _Height(Node *root){
       if(root==nullptr){
           return 0;
       }
       int leftHeight=_Height(root->_left);
       int rightHeight=_Height(root->_right);
       return leftHeight>rightHeight ? leftHeight+1 : rightHeight+1;
   }
   int _Size(Node *root){
       if(root==nullptr){
           return 0;
       }
       int leftsize=_Size(root->_left);
       int rightsize=_Size(root->right);
       return leftsize+rightsize+1;
   }
   void _InOrder(Node *root){
       if(root==nullptr){
           return;
       }
       _InOrder(root->_left);
       cout<<root->_kv.first<<" : "<<root->_kv.second<<endl;
       _InOrder(root->_right);
   }
    Node *_root=nullptr;
};

void TestRBTree1()
{
	RbTree<int, int> t;

	//常规测试用例：（单旋场景）
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	//特殊的需要双旋场景的测试用例：（双旋场景）
	//int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	//插入
	for (auto e : a)
	{
		t.insert({ e,e });
	}
    if(t.isBalance()){
        cout<<"红黑树已经平衡！！！"<<endl;
    }else{
        cout<<"红黑树不是合法的！"<<endl;
    }
	//中序遍历
	t.InOrder();
}
int main()
{
    TestRBTree1();
    return 0;
}

