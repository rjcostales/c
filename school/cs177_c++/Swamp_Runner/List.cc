/*	Raphael J. S. Costale
 *	List.cc
 

 
 
 

 
 
 
     List::Element *curr = pList.mHead;

     pOstream <<"( ";
     
     	pOstream << curr->mData << ((curr = curr->mNext) ? ", " : " ");
        
        
}




    List bufList(*this);
    for (int i = 0; i < pList.mSize; i++)
        bufList.append(pList[i]);
    return bufList;



{
    if (this != &pList)
        
        
        
        
    
    




    return atIndex(pIndex);



{
    return atIndex(pIndex);




    Element *curr = mHead;
    
        for (int i = 0; i < mSize; i++)
        {
            if (operator[](i) != pList[i])
            return 0;
            curr = curr->mNext;
        }
        return 1;
    }




    return !(*this == pList);
}




    if (this != &pList)
        mHead = 0;
        mSize = 0;
        
        while (i--)
        
    }




    clear();





    Element *curr = mHead;
    
        mHead = mHead->mNext;
        
        curr = mHead;
    }
    mHead = 0;
    mSize = 0;




    Element *curr = mHead, *newElem = new Element(pData);

    
        if ((pIndex >= 0)&&(pIndex <= mSize))
            if (pIndex)
                while (--pIndex)
                

                newElem->mNext = curr->mNext;


                newElem->mNext = mHead;
        
    
	   }
    

        mHead = newElem;

    mSize++;
    
}


    return insert(pData, mSize);




	Element *curr = mHead, *temp = mHead;
    Type tmpData;

    if (curr)
        if ((pIndex >= 0)&&(pIndex < mSize))
            if (pIndex)
                while(--pIndex)
                    curr = curr->mNext;

                temp = curr->mNext;
                curr->mNext = temp->mNext;
            }
                mHead = curr->mNext;

            tmpData = temp->mData;
            delete temp;
            mSize--;
        }
            error("remove(): subscript out of range\n");
    }
        error("remove(): List is empty\n");
    return tmpData;




    Element *temp = mHead, *curr = mHead;
    
        
            if (curr == mHead)
                mHead = curr->mNext;
                delete curr;
                curr = mHead;
            }
                temp->mNext = curr->mNext;
                delete curr;
                curr = temp->mNext;
            }

            mSize--;
        }
            temp = curr;
            curr = curr->mNext;
        }
    }

    return *this;



    return mSize;



    Element *curr = mHead;
    
    
        
        
        curr = curr->mNext;
    }
    return count;



    Element *curr = mHead;

    if (curr)
        if ((pIndex >= 0)&&(pIndex < mSize))
            
                curr = curr->mNext;
        }
            error("operator[]: subscript out of range\n");
    }
        error("operator[]: List is empty\n");
    return curr->mData;



    cerr << errMessage;
    


