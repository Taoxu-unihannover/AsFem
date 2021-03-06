//****************************************************************
//* This file is part of the AsFem framework
//* A Simple Finite Element Method program (AsFem)
//* All rights reserved, Yang Bai @ CopyRight 2020
//* https://github.com/yangbai90/AsFem.git
//* Licensed under GNU GPLv3, please see LICENSE for details
//* https://www.gnu.org/licenses/gpl-3.0.en.html
//****************************************************************

#include "Mesh/Mesh.h"
#include "Utils/StringUtils.h"

//*************************************************************************
//*** For all the functions listed here, we assume the inp file name
//*** is correct and can be opened successuflly.
//*************************************************************************

int Mesh::GetAbaqusNodesNumFromInp(string filename) const{
    ifstream in;
    string str;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int nNodes=0;
    while(!in.eof()){
        if(str.find("*Node")!=string::npos){
            nNodes=0;
            getline(in,str);// already get first node's coordinates
            while(str.find("*Element")==string::npos){
                nNodes+=1;
                getline(in,str);
            }
            break;
        }
        getline(in,str);
    }
    in.close();
    return nNodes;
}

//****************************************************
int Mesh::GetAbaqusElmtsNumFromInp(string filename) const{
    ifstream in;
    string str;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int nElmts=0;
    while(!in.eof()){
        if(str.find("*Element")!=string::npos){
            nElmts=0;
            getline(in,str);// already get first node's coordinates
            while(str.find("*End")==string::npos){
                nElmts+=1;
                getline(in,str);
            }
            break;
        }
        getline(in,str);
    }
    in.close();
    return nElmts;
}
//****************************************************
int Mesh::GetAbaqusBCElmtsNumFromInp(string filename) const{
    ifstream in;
    string str;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int nElmts=0;
    vector<double> numbers;
    nElmts=0;
    while(!in.eof()){
        if(str.find("*Elset")!=string::npos){
            if(str.find("elset=Set-")==string::npos){
                // only account for the bc elements
                getline(in,str);
                while(str.find("*")==string::npos){
                    numbers=SplitStrNum(str);
                    if (numbers.size()==3){
                        // for the incremental case,i.e. 1,7,1 (we have seven elements)
                        nElmts+=int(numbers[2 - 1]);
                    }
                    else if(numbers.size()<1){
                        PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                        PetscPrintf(PETSC_COMM_WORLD,"*** Error: invalid element index number in you inp file !!!           ***\n");
                        PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                        Msg_AsFem_Exit();
                    }
                    else{
                        nElmts+=numbers.size();
                    }
                    getline(in,str);
                }
            }
        }
        getline(in,str);
    }
    in.close();
    return nElmts;
}

//****************************************************
int Mesh::GetAbaqusBCElmtNodesNumFromInp(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return 2;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return 3;
    }
    else{
        return -1;
    }
}
//****************************************************
int Mesh::GetElmtNodesNumFromInpElmtName(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return 4;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return 8;
    }
    else{
        return -1;
    }
}
//****************************************************
int Mesh::GetVTKCellTypeFormInpMeshTypeName(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return 9;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return 23;
    }
    else{
        return -1;
    }
}
//****************************************************
int Mesh::GetElmtOrderViaInpElmtTypeName(string meshtypename)const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return 1;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return 2;
    }
    else{
        return -1;
    }
}
//****************************************************
//****************************************************
int Mesh::GetDimFromInpMeshTypeName(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return 2;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return 2;
    }
    else{
        return -1;
    }
}
//****************************************************
MeshType Mesh::GetMeshTypeViaAbaqusMeshName(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return MeshType::QUAD4;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return MeshType::QUAD8;
    }
    else{
        return MeshType::NULLTYPE;
    }
}
//****************************************************
MeshType Mesh::GetBCMeshTypeViaAbaqusMeshName(string meshtypename) const{
    if(meshtypename.find("CPS4R")!=string::npos){
        return MeshType::EDGE2;
    }
    else if(meshtypename.find("CPS8R")!=string::npos){
        return MeshType::EDGE3;
    }
    else{
        return MeshType::NULLTYPE;
    }
}
//****************************************************
int Mesh::GetNodeSetsNumFromInp(string filename) const{
    ifstream in;
    string str;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int nNodeSets=0;
    while(!in.eof()){
        if(str.find("*Nset")!=string::npos&&str.find("nset=Set")==string::npos){
            nNodeSets+=1;
        }
        getline(in,str);
    }
    in.close();
    return nNodeSets;
}

//****************************************************
int Mesh::GetElmtSetsNumFromInp(string filename) const{
    ifstream in;
    string str;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int nElmtSets=0;
    while(!in.eof()){
        if(str.find("*Elset")!=string::npos&&str.find("elset=Set")==string::npos){
            nElmtSets+=1;
        }
        getline(in,str);
    }
    in.close();
    return nElmtSets;
}

//********************************************
string Mesh::GetElmtTypeNameFromInp(string filename) const{
    ifstream in;
    string str,substr;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    int i;
    substr.clear();
    while(!in.eof()){
        if(str.find("*Element,")!=string::npos){
            i=str.find_last_of("=");
            substr=str.substr(i,str.length()+1);
            substr=RemoveStrSpace(substr);
            break;
        }
        getline(in,str);
    }
    in.close();
    return substr;
}

//************************************
vector<string> Mesh::GetNodeSetsNameFromInp(string filename) const{
    ifstream in;
    string str,substr,tempstr;
    vector<string> namelist,strvec;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    substr.clear();
    namelist.clear();
    while(!in.eof()){
        if(str.find("*Nset,")!=string::npos&&str.find("nset=Set")==string::npos){
            strvec=SplitStr(str,',');
            if(strvec.size()>=3){
                //have all the necessary information
                tempstr=strvec[2-1];
                substr=tempstr.substr(tempstr.find("=")+1);
                substr=RemoveStrSpace(substr);
                namelist.push_back(substr);
            }
            else{
                PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find nset= name in your inp file !!!                  ***\n");
                PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                Msg_AsFem_Exit();
            }
        }
        getline(in,str);
    }
    in.close();
    return namelist;
}

//************************************
vector<string> Mesh::GetElmtSetsNameFromInp(string filename) const{
    ifstream in;
    string str,substr,tempstr;
    vector<string> namelist,strvec;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    substr.clear();
    namelist.clear();
    while(!in.eof()){
        if(str.find("*Elset,")!=string::npos&&str.find("elset=Set")==string::npos){
            strvec=SplitStr(str,',');
            // cout<<"str="<<str<<endl;
            // for(auto it:strvec){
            //     cout<<it<<endl;
            // }
            if(strvec.size()>=3){
                //have all the necessary information
                tempstr=strvec[2-1];
                substr=tempstr.substr(tempstr.find("=")+1);
                substr=RemoveStrSpace(substr);
                namelist.push_back(substr);
            }
            else{
                PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find elset= name in your inp file !!!                 ***\n");
                PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                Msg_AsFem_Exit();
            }
        }
        getline(in,str);
    }
    in.close();
    return namelist;
}

//************************************************
vector<vector<int>> Mesh::GetNodeIndexSetsFromInp(string filename) const{
    ifstream in;
    string str;
    vector<vector<int>> NodeIndexSets;
    vector<int> tempindex;
    vector<double> numbers;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    
    NodeIndexSets.clear();
    while(!in.eof()){
        if(str.find("*Nset,")!=string::npos&&str.find("nset=Set")==string::npos){
            getline(in,str);
            while(str.find("*")==string::npos){
                tempindex.clear();
                numbers=SplitStrNum(str);
                if(numbers.size()==3){
                    for(int i=int(numbers[0]);i<=int(numbers[1]);i+=int(numbers[2])){
                        tempindex.push_back(i);
                    }
                    NodeIndexSets.push_back(tempindex);
                }
                else if(numbers.size()<1){
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find node set index number in your inp file !!!       ***\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    Msg_AsFem_Exit();
                }
                else{
                    for(int i=0;i<(int)numbers.size();i++){
                        tempindex.push_back(int(numbers[i]));
                    }
                    NodeIndexSets.push_back(tempindex);
                }
            }
        }
        getline(in,str);
    }
    in.close();
    return NodeIndexSets;
}
vector<vector<int>> Mesh::GetElmtIndexSetsFromInp(string filename) const{
    ifstream in;
    string str;
    vector<vector<int>> ElmtIndexSets;
    vector<int> tempindex;
    vector<double> numbers;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    
    ElmtIndexSets.clear();
    while(!in.eof()){
        if(str.find("*Elset,")!=string::npos&&str.find("elset=Set")==string::npos){
            getline(in,str);
            while(str.find("*")==string::npos){
                tempindex.clear();
                numbers=SplitStrNum(str);
                if(numbers.size()==3){
                    for(int i=int(numbers[0]);i<=int(numbers[1]);i+=int(numbers[2])){
                        tempindex.push_back(i);
                    }
                    ElmtIndexSets.push_back(tempindex);
                }
                else if(numbers.size()<1){
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find elmt set index number in your inp file !!!       ***\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    Msg_AsFem_Exit();
                }
                else{
                    for(int i=0;i<(int)numbers.size();i++){
                        tempindex.push_back(int(numbers[i]));
                    }
                    ElmtIndexSets.push_back(tempindex);
                }
            }
        }
        getline(in,str);
    }
    in.close();
    return ElmtIndexSets;
}

//*********************************************
vector<int> Mesh::GetNodeIndexVecFromInpNodeSetName(string filename,string nodesetname)const{
    ifstream in;
    string str;
    vector<int> NodeIndexSets;
    vector<double> numbers;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    
    NodeIndexSets.clear();
    while(!in.eof()){
        if(str.find("*Nset,")!=string::npos&&str.find(nodesetname)!=string::npos){
            getline(in,str);
            while(str.find("*")==string::npos){
                numbers=SplitStrNum(str);
                if(numbers.size()==3){
                    for(int i=int(numbers[0]);i<=int(numbers[1]);i+=int(numbers[2])){
                        NodeIndexSets.push_back(i);
                    }
                }
                else if(numbers.size()<1){
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find elmt set index number in your inp file !!!       ***\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    Msg_AsFem_Exit();
                }
                else{
                    for(int i=0;i<(int)numbers.size();i++){
                        NodeIndexSets.push_back(int(numbers[i]));
                    }
                }
                getline(in,str);
            }
        }
        getline(in,str);
    }
    in.close();
    return NodeIndexSets;
}
//************************************************
vector<int> Mesh::GetElmtIndexVecFromInpNodeSetName(string filename,string elmtsetname)const{
    ifstream in;
    string str;
    vector<int> ElmtIndexSets;
    vector<double> numbers;
    in.open(filename.c_str(),ios::in);
    getline(in,str);
    
    ElmtIndexSets.clear();
    while(!in.eof()){
        if(str.find("*Elset,")!=string::npos&&str.find(elmtsetname)!=string::npos){
            getline(in,str);
            while(str.find("*")==string::npos){
                numbers=SplitStrNum(str);
                if(numbers.size()==3){
                    for(int i=int(numbers[0]);i<=int(numbers[1]);i+=int(numbers[2])){
                        ElmtIndexSets.push_back(i);
                    }
                }
                else if(numbers.size()<1){
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*** Error: cant find elmt set index number in your inp file !!!       ***\n");
                    PetscPrintf(PETSC_COMM_WORLD,"*************************************************************************\n");
                    Msg_AsFem_Exit();
                }
                else{
                    for(int i=0;i<(int)numbers.size();i++){
                        ElmtIndexSets.push_back(int(numbers[i]));
                    }
                }
                getline(in,str);
            }
        }
        getline(in,str);
    }
    in.close();
    return ElmtIndexSets;
}