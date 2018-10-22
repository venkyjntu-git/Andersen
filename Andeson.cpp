#include<map>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#define ID  256
using namespace std;
map<int,vector<int>> ptsset;
map<string,int> symbol_table;
bool change = true;

bool isfound(vector<int> v, int elm) {

 std::vector<int>::iterator x = v.begin();
    while(x!=v.end())   { 
       if(*x==elm) return true;
       x++;
    }
    return false;
} 
string idToName(int x) {
  std::map<string,int>::iterator ss= symbol_table.begin();

                           while(ss!=symbol_table.end()) {

                            if(ss->second==x) return ss->first;
                                ss++;
                             }

}
int current_id=0;
int constg_iter=1;
 struct Edge {
  int src;
  int dest;
 };

 class ConsGraph {
   vector<int> *adjList;
  public: 
  ConsGraph() { 
    adjList = new vector<int>[100]; 
     }
   void insertedge(int src,int dest) {
       adjList[src].push_back(dest);
      }
   ~ConsGraph() {
        delete[] adjList;
      }
    void printGraph() {
         // cout<<"constraint Graph edges after "<< constg_iter++ <<" iteration"<<endl;
       std::map<string,int>::iterator b= symbol_table.begin();
               //if(b!=symbol_table.end()) cout<<"Important constraints (x,y) mean x is a subset of y\n";
          while(b!=symbol_table.end()) {
            std::vector<int>::iterator v= adjList[b->second].begin();   
             while(v!=adjList[b->second].end()) {
                cout<<"("<<idToName(b->second)<<","<<idToName(*v)<<")"<<endl;
                     v++;
          }
            b++;          
       }
   }

    void Propagate() {
              
               std::map<string,int>::iterator b= symbol_table.begin();
          while(b!=symbol_table.end()) {
            std::vector<int>::iterator v= adjList[b->second].begin();
                       std::map<int,vector<int>>::iterator pb = ptsset.find(b->second);
                 if( pb != ptsset.end()) { 
                        
             while(v!=adjList[b->second].end()) {
                  std::vector<int>::iterator pset= pb->second.begin();
                            while(pset!=pb->second.end()) {
                                      if(!isfound(ptsset[*v],*pset)) {
                                
                                      ptsset[*v].push_back(*pset);
                                        change=true;
                                     }
                                          pset++;
                       }
                v++;
              }
            }
             b++;
       }
   }
     
}ConsG;

string stmt[100],left,right;
int typeofstmt[100]; 
std::string::iterator it;
std::string::iterator last;
//map<string,string> symbol_table;

bool isalpha(char c) {

  if ((c>=65 && c<=90) || (c>=97 && c <=122))
      return true;
   else return false;

}
 bool isalphanum(char c) {

  if(isalpha(c) || (c>=48 && c<=57)|| (c=='_'))
       return true;
  else
       return false;
}

int nexttoken(int nums,int d=0) {
                int state =0;
                  string lexeme;
    while(it!=stmt[nums].end()) {
                       //cout<<state<<"->"<<*it;         
                 switch(state)  {
                  case 0: 
                          if(*it == ' ') { it++; state=0; }
                          if(*it == '*')  state=1;
                          else if (*it == '&') state=2;
                           else if (*it == '=') state=3;
                            else if (isalpha(*it)) { 
                                                        lexeme.push_back(*it); 
                                                         it++;  
                                                      if(it==stmt[nums].end()) {
                                                                   std::map<string,int>::iterator sit= symbol_table.find(lexeme);
                                                                           if(sit==symbol_table.end()) {
                                                                                  //vector<int> v;
                                                                          symbol_table[lexeme] = ++current_id;
                                                                             //ptsset[current_id-1] = v;
                                                                          }
                                                                    if(d==1)  return symbol_table[lexeme];
                                                                    
                                                                   return ID;
                                                           }
                                                          state=4;
                                                    }
                            break;
                  case 1:  it++;  return '*';
                             break;
                  case 2: it++; return '&';
                             break;
                  case 3: it++;  return '=';
                             break;
                  case 4:
                             if(isalphanum(*it)) { lexeme.push_back(*it); it++; state =4;} 
                           else {  
                                  // cout<<endl<<lexeme<<"id matched"<<endl;
                                         std::map<string,int>::iterator sit= symbol_table.find(lexeme);
                                            if(sit==symbol_table.end()) {
                                                   //vector<int> v;
                                                symbol_table[lexeme] = ++current_id;
                                                 //ptsset[current_id-1] = v;
                                                } 
                                        if(d==1)  return symbol_table[lexeme]; 
                                          
                                       return ID; 
                                 }   
                              break;                          
               }
   
    }
}
 
int S(int nums) {

   //rule 1,2,3,4 ID=ID,ID=*ID,ID=&ID,*ID=ID
          it=stmt[nums].begin();
   if(nexttoken(nums)==ID && nexttoken(nums)=='='&&nexttoken(nums)==ID) return 1;
           //cout<<"rule2"<<endl;
           it=stmt[nums].begin();
   if(nexttoken(nums)==ID && nexttoken(nums)=='='&& nexttoken(nums)=='*' && nexttoken(nums)==ID) return 2;
           it=stmt[nums].begin();
     if(nexttoken(nums)==ID && nexttoken(nums)=='='&& nexttoken(nums)=='&' && nexttoken(nums)==ID) return 3;
           it=stmt[nums].begin();
     if(nexttoken(nums)=='*' && nexttoken(nums)==ID && nexttoken(nums)=='='&&nexttoken(nums)==ID) return 4;

   else return -1;
     
      
}
 int Iterations=1;
void printPointstoSets(char *outf)
{
        ofstream fo; 
     fo.open(outf,ios::out);
            fo<< " digraph D { \n";

   // cout<<" Pointsto Sets after iteration "<<Iterations++ <<endl;
      cout<<" Pointsto Set "<<endl;
       std::map<string,int>::iterator b= symbol_table.begin();
                     
          while(b!=symbol_table.end())
             {
           cout<< b->first<<" {";
              fo<< b->first << "  -> {";
         
                   std::map<int,vector<int>>::iterator bs = ptsset.find(b->second);
                       if(bs!= ptsset.end())
                    { 
                   std::vector<int>::iterator bv = bs->second.begin();
                     while(bv!=bs->second.end()) {
                          cout<<idToName(*bv);
                           fo<<idToName(*bv);
                          bv++;
                          if(bv!=bs->second.end()) { cout<<",";  fo<<", ";}
                   }
                     
                   }     
                  cout<<"}"<<endl;
                   fo<<"} \n";
                    b++;
              }
    fo<<"}\n";
}



 int number_addresstaken_stmts=0;
int main(int argv, char *args[]) {
 
  ifstream fio;
 
   string line; 
  //fio.open("ex.c",ios::in);
   fio.open(args[1],  ios::in); 
  
    int numstmts=0,i;
            if(! fio.is_open())  return -1;
    
  while (getline(fio, stmt[numstmts])) { 
                     int t=S(numstmts);
             //cout<<t;
                 if(t==-1) { cout<<"statement " << stmt[numstmts]<<" either not a valid 3-address code or not supported by the current implementation \n"; continue;}
                     if (t==3) {
                                 // cout<<stmt[numstmts]<<" "<<endl;
                                it=stmt[numstmts].begin();
                                  int idl= nexttoken(numstmts,1);
                                   int c1 = nexttoken(numstmts);
                                   int c2 = nexttoken(numstmts);
                                   int idr = nexttoken(numstmts,1);
                                //cout<<idl<<endl;   
                             // std::map<int,vector<int>>::iterator b= ptsset.find(idl);
                                // cout<<idr<<endl;
                                         if(isfound(ptsset[idl],idr)==false)
                                        ptsset[idl].push_back(idr);                                                     
                                       number_addresstaken_stmts++;
                                      continue;
                               }
               // processing copy statements
          
                       if(t==1) {
                               it=stmt[numstmts].begin();
                                  int idl= nexttoken(numstmts,1);
                                    int c1 = nexttoken(numstmts);
                                    int idr = nexttoken(numstmts,1);
                         ConsG.insertedge(idr,idl);
                           continue;
                         }
                                         
                   numstmts++;  
                  typeofstmt[numstmts-1] =t; 
         // Print line in Console 
        //cout << stmt[numstmts-1] << endl;

    } 


   //printPointstoSets();
// cout<<"constraint Graph edges after 1 iteration"<<endl;
   //ConsG.printGraph(); 
    ConsG.Propagate();
  
 while(change) {
     
     change = false;
   
   //printPointstoSets();
         
        for(i=0;i<numstmts;i++) {

               if(typeofstmt[i] == 2) {
                         it=stmt[i].begin();
                                  int idl= nexttoken(i,1);
                                   int c1 = nexttoken(i);
                                   int c2 = nexttoken(i);
                                   int idr = nexttoken(i,1);               
                                 //cout<<idl<<idr<<endl;
                          std::vector<int>::iterator v= ptsset[idr].begin();
                                   while(v!=ptsset[idr].end()) {
                                        ConsG.insertedge(*v,idl);
                                      v++;
                                }
                         
                                     
                  }

               else if (typeofstmt[i]==4) {
                                          it=stmt[i].begin();
                           
                                     int c1 = nexttoken(i);
                                  int idl= nexttoken(i,1);
                                   int c2 = nexttoken(i);
                                   int idr = nexttoken(i,1);

                          std::vector<int>::iterator v= ptsset[idl].begin();
                                   while(v!=ptsset[idl].end()) {
                                             //cout<<idl<<idr<<endl;
                                        ConsG.insertedge(idr,*v);
                                      v++;
                                }


                }
         }//endfor
              ConsG.Propagate();
        // ConsG.printGraph();  
//    change=false;
  } //endwhile

//ConsG.Propagate();
//   printPointstoSets();

      // ConsG.printGraph(); 
      printPointstoSets(args[2]);
} //endmain



