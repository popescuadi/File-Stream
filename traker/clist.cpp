#include "clist.h"

CList::CList()
{

}
void CList::push_back(char *ipaddr,char *files)
{
    Nod *contor;
    if (head==nullptr)
    {
        head=new Nod(ipaddr);
        contor=head;
    }
    else
    {

        contor=head;
        while (contor->next!=nullptr)
            contor=contor->next;
        contor->next=new Nod(ipaddr);
        contor->next->prev=contor;
        contor=contor->next;
    }
    char *token=NULL;
    if (files!=NULL)
    {
    token=strtok(files,"|");
    while (token!=NULL)
    {
        contor->filesList.push_back(Utilitis::my_strdup(token));
        token=strtok(NULL,"|");
    }
    }
}
bool CList::Search(char *buffer, int poz)
{
    int dim=this->size();
    if (poz<0 || poz>=dim)
        return false;
    else
    {
        int contor=0;
        Nod *index=head;
        while (index!=nullptr)
        {
            if (poz==contor)
               return index->Search(buffer);
            index=index->next;

        }
    }
}

void CList::addFile(char *ip, char *file)
{
    Nod *index;
    index=head;
    while (index!=nullptr)
    {
        if (strcmp(index->ip,ip)==0)
        {
            index->filesList.push_back(Utilitis::my_strdup(file));
            break;
        }
        index=index->next;
    }
}

void CList::renew(char *ip)
{
    Nod *index;
    index=head;
    while (index!=nullptr)
    {
        if (strcmp(index->ip,ip)==0)
        {
            index->alive=180;
            break;
        }
        index=index->next;
    }
}

int CList::size()
{
    int dim=0;
    Nod *index;
    index=head;
    while (index!=nullptr)
    {
        dim++;
        index=index->next;
    }
    return dim;
}
void CList::pop_up(char *ipaddr)
{
    Nod *index;
  if (strcmp(head->ip,ipaddr)==0)
  {
      index=head;
      head=head->next;
      head->prev=nullptr;
      delete index;
  }

  else
  {
      index=head->next;
      while (index!=nullptr)
      {
      if (strcmp(index->ip,ipaddr)==0)
      {
          index->prev->next=index->next;
          if (index->next!=nullptr)
              index->next->prev=index->prev;
          break;
      }
      index=index->next;
      }
  }
}
char *CList::operator [](int poz)
{
    if (poz <0 || poz >this->size())
        return nullptr;
    else
    {
        Nod *index;
        index=head;
        int poz_intern=0;
        while (index!=nullptr)
        {

            if (poz_intern==poz)
                return index->ip;
            index=index->next;
            poz_intern++;
        }
    }
}
