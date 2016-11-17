#include <vector>
#include "windows.h"
#include "resource.h"
#include <string>
#include <strstream>

using namespace std;

class Visitor;
class Model;
class View;
class Controller;

class Kcoll
{
public:
	virtual void accept(Visitor &visitor) = 0;
};

class Kset : public Kcoll
{

    vector<int> mas;

public:

	void accept(Visitor &visitor);

    Kset()
    {};
 
    int insert(int nw)
    {
        if (!mas.size())
        {
            mas.push_back(nw);
            return 0;
        }
        else
        {
            int l = 0, r = mas.size() - 1;
            int t = (l + r) / 2;
            if (nw > mas[r])
            {
                mas.push_back(nw);
                return 0;
            }
            if (nw < mas[l])
            {
                mas.insert(mas.begin(), nw);
                return 0;
            }
            while ( t != l )
            {
                if (nw > mas[t])
                    l += (r - l) / 2;
                if (nw < mas[t])
                    r -= (r - l) / 2;
                t = (l + r) / 2;
                if (nw == mas[t])
                    return 1;
            }
            if ((nw > mas[t])&&(nw < mas[t+1]))
            {
                mas.push_back(mas[mas.size() - 1]);
                for (int i = mas.size() - 1; i > t + 1; i--)
                    mas[i] = mas[i-1];
                mas[t+1] = nw;
                return 0;
            }
        }
    };
       
    Kset(int *a, int b)
    {
        for (int i = 0; i < b; i++)
            (*this).insert(a[i]);
    };
 
    Kset(const Kset &c)
    {
        mas = c.mas;
    };
 
    int size() const
    {
        return mas.size();
    };
 
    void clear()
    {
        mas.clear();
    };
 
    void operator= (const Kset &c)
    {
        mas = c.mas;
    };
 
    void swap(Kset b)
    {
        Kset tempKset;
        tempKset = (*this);
        (*this) = b;
        b = (*this);
    };
 
    Kset operator+ (const Kset &b) //union of sets
    {
        Kset uni;
        uni = *this;
        int n = b.size();
        for (int i = 0; i < n; i++)
            uni.insert(b.mas[i]);
        return uni;
    };
 
    Kset &operator+= (const Kset &b)
    {
        (*this) = ((*this) + b);
        return *this;
    };
 
    Kset operator* (const Kset &b) //intersection of sets
    {
        Kset tempKset = *this;
        int n = b.size();
        Kset inter;
        for (int i = 0; i < n; i++)
            if (tempKset.insert(b.mas[i]))
                inter.insert(b.mas[i]);
        return inter;
    };
   
    Kset &operator*= (const Kset &b)
    {
        (*this) = ((*this) * b);
        return *this;
    };
 
    Kset operator/ (const Kset &b) //difference of sets
    {
        Kset inter;
        inter = (*this) * b;
        Kset diff;
        int n = (*this).size();
        for (int i = 0; i < n; i++)
            if (!inter.insert((*this).mas[i]))
                diff.insert((*this).mas[i]);
        return diff;
    };
 
    Kset &operator/= (const Kset &b)
    {
        (*this) = ((*this) / b);
        return *this;
    };
 
    Kset operator- (const Kset &b) //symmetric difference of sets
    {
        Kset symDiff;
        symDiff = (*this) + b;
        symDiff /= ((*this) * b);
        return symDiff;
    };
 
    Kset &operator-= (const Kset &b)
    {
        (*this) = ((*this) - b);
        return *this;
    };
 
    bool operator== (const Kset &b)
    {
        Kset uni;
        uni = (*this) + b;
        Kset inter;
        inter = (*this) * b;
        if ((*this + b).size() == (*this * b).size())
            return true;
        else
            return false;
       
    };
   
    bool operator!= (const Kset &b)
    {
        return !((*this) == b);
    };
 
    bool operator< (const Kset &b) // if this belongs to b
    {
        Kset inter;
        inter = (*this) * b;
        if (inter == (*this))
            return true;
        else
            return false;                
    };
 
        class iterator
        {
 
            int *ptr;
 
        public:
 
            iterator()
            {
                ptr = NULL;
            };
 
            iterator(int *a)
            {
                ptr = a;
            };
 
            void operator++()
            {
                ptr++;
            };
 
            void operator= (const iterator &itr)
            {
                if (&ptr != &(itr.ptr))
                    ptr = itr.ptr;
            };
 
            int &operator*()
            {
                return *ptr;
            };
       
            bool operator== (const iterator &itr)
            {
                return ptr == itr.ptr;
            };
 
            bool operator!= (const iterator &itr)
            {
                return !(ptr == itr.ptr);
            };
 
        };
 
    iterator begin()
    {
        if (mas.size())
        {
            int *p = &(*mas.begin());
            iterator temp(p);
            return temp;
        }
        else
        {
            iterator temp;
            return temp;
        };
    };
 
    iterator end()
    {
        if (mas.size())
        {
            int *p = &(mas[mas.size()-1]) + 1;
            iterator temp(p);
            return temp;
        }
        else
        {
            iterator temp;
            return temp;
        };
    };

	void getFromStr(char * str)
	{
		strstream istr;
		istr << str;
		int temp;
		while (istr >> temp)
			(*this).insert(temp);
	};

	void toStr(string &str)
	{
		strstream sstr;
		for(iterator it = (*this).begin(); it != (*this).end(); ++it)
		{
			sstr << *it;
			sstr << " ";
		}
		string temp;
		while (sstr >> temp)
		{
			str += temp;
			str += " ";
		}
	};
 
};
 
class Visitor
{
public:

	virtual void visit(Kset &kset) = 0;
};

class Increment : public Visitor
{
public:

	void visit(Kset &kset)
	{
		for (Kset::iterator it = kset.begin(); it != kset.end(); ++it)
			(*it)++;
	};

};

class Decrement : public Visitor
{
public:

	void visit(Kset &kset)
	{
		for (Kset::iterator it = kset.begin(); it != kset.end(); ++it)
			(*it)--;
	};
};

void Kset::accept(Visitor &visitor)
{
	visitor.visit(*this);
};

bool operator<= (int a, Kset &b)
{
    Kset tempKset;
    tempKset.insert(a);
    return (tempKset < b);
}

void PutToList(Kset set,HWND hWnd, int listId)
{
	string out;
	set.toStr(out);
	SendMessage(GetDlgItem(hWnd, listId), LB_DELETESTRING, 0, NULL);
	SendMessage(GetDlgItem(hWnd, listId), LB_ADDSTRING, NULL, (LPARAM)out.c_str());
}

enum idc {UPDIFSTR, UPDSET1, UPDSET2, UPDSET3};

class Model
{

	Kset set1;
	Kset set2;
	Kset set3;
	char * ifstr;

public:

	View * viewptr;
	Controller * controllerptr;

	void getMessage(WPARAM wParam, LPARAM lParam);
	
	friend class View;
};


class View
{
	HWND hwnd;
	
public:

	Model * modelptr;
	Controller * controllerptr;

	void initView(HINSTANCE hInstance, int Id, DLGPROC funcname)
	{
		hwnd = CreateDialog(
			  hInstance,
			  MAKEINTRESOURCE(Id),
			  NULL,
			  funcname
		);
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) 
		{   	
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	};

	HWND retwnd()
	{
		return hwnd;
	};

	void getMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	void getModelChanges(idc idch)
	{
		switch(idch)
		{
			case UPDIFSTR:
				SendMessage(GetDlgItem(hwnd, IDC_IF_ANSWER), LB_DELETESTRING, 0, NULL);
				SendMessage(GetDlgItem(hwnd, IDC_IF_ANSWER), LB_ADDSTRING, NULL, (LPARAM)modelptr->ifstr);
			break;
			case UPDSET1:
				PutToList(modelptr->set1, hwnd, IDC_LIST_SET1);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT_SET1), WM_SETTEXT, 0, (LPARAM)"");
			break;
			case UPDSET2:
				PutToList(modelptr->set2, hwnd, IDC_LIST_SET2);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT_SET2), WM_SETTEXT, 0, (LPARAM)"");
			break;
			case UPDSET3:
				PutToList(modelptr->set3, hwnd, IDC_LIST_SET3);
			break;
		}
	};

	void getViewChanges(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		DefWindowProc(hwnd,msg,wParam,lParam);
	};



};

class Controller
{
	

public:

	Model * modelptr;
	View * viewptr;

	void directMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		
		switch (msg)
		{
			case WM_COMMAND:
				modelptr->getMessage(wParam, lParam);
			break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				viewptr->getViewChanges(msg,wParam,lParam);		
		}
	};

};

void View::getMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	controllerptr->directMessage(msg, wParam, lParam);
};

void Model::getMessage(WPARAM wParam, LPARAM lParam)
	{
			switch(LOWORD(wParam))
				{
				case IDC_IF_EQUAL:
					{
						if (set1 == set2)
							ifstr = "Sets are equal";	
						else
							ifstr = "Sets are not equal";
						viewptr->getModelChanges(UPDIFSTR);
					}
				break;
				case IDC_IF_BELONG1:
					{			
						if (set1 < set2)
							ifstr = "Set1 belongs to Set2";
						else
							ifstr = "Set1 does not belong to Set2";	
						viewptr->getModelChanges(UPDIFSTR);
					}
				break;
				case IDC_IF_BELONG2:
					{				
						if (set2 < set1)
							ifstr = "Set2 belongs to Set1";		
						else
							ifstr = "Set2 does not belong to Set1";
						viewptr->getModelChanges(UPDIFSTR);
					}
				break;
				case IDC_ADD1:
					{
						char input[50];
						SendMessage(GetDlgItem(viewptr->retwnd(), IDC_EDIT_SET1), EM_GETLINE, 0, (LPARAM)input);
						set1.getFromStr(input);
						viewptr->getModelChanges(UPDSET1);
					}
				break;
				case IDC_ADD2:
					{
						char input[50];
						SendMessage(GetDlgItem(viewptr->retwnd(), IDC_EDIT_SET2), EM_GETLINE, 0, (LPARAM)input);
						set2.getFromStr(input);
						viewptr->getModelChanges(UPDSET2);
					}
				break;
				case IDC_INTERSECTION:
					{
						set3 = (set1 * set2);
						viewptr->getModelChanges(UPDSET3);
					}
				break;
				case IDC_UNION:
					{
						set3 = (set1 + set2);
						viewptr->getModelChanges(UPDSET3);
					}
				break;
				case IDC_DIFFERENCE:
					{
						set3 = (set1 / set2);
						viewptr->getModelChanges(UPDSET3);
					}
				break;
				case IDC_SYMMETRICDIFFERENCE:
					{
						set3 = (set1 - set2);
						viewptr->getModelChanges(UPDSET3);
					}
				break;
				case IDC_CLEAR1:
					{
						set1.clear();
						viewptr->getModelChanges(UPDSET1);
					}
				break;
				case IDC_CLEAR2:
					{
						set2.clear();
						viewptr->getModelChanges(UPDSET2);
					}
				break;
				case IDC_INC1:
					{
						Increment incr;
						set1.accept(incr);
						viewptr->getModelChanges(UPDSET1);
					}
				break;
				case IDC_INC2:
					{
						Increment incr;
						set2.accept(incr);
						viewptr->getModelChanges(UPDSET2);
					}
				break;
				case IDC_DEC1:
					{
						Decrement decr;
						set1.accept(decr);
						viewptr->getModelChanges(UPDSET1);
					}
				break;
				case IDC_DEC2:
					{
						Decrement decr;
						set2.accept(decr);
						viewptr->getModelChanges(UPDSET2);
					}
				break;
				}
			
};

Model mainModel;
View mainView;
Controller mainController;

INT_PTR CALLBACK MainDlgFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{  
	static Kset set1;
	static Kset set2;
	static Kset set3;
	mainController.directMessage(msg, wParam, lParam);
    return (0);
}
 
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE HPrevInst, LPSTR lpCmdLine, int nCmdShow)
{

	mainModel.viewptr = &mainView;
	mainModel.controllerptr = &mainController;
	mainView.controllerptr = &mainController;
	mainView.modelptr = &mainModel;
	mainController.modelptr = &mainModel;
	mainController.viewptr = &mainView;
	mainView.initView(hInstance, IDD_MAINDIALOG, MainDlgFunc);
	return 0;
}