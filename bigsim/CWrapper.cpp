#include "entities/MsgEntry.h"
#include "entities/PE.h"
#include "CWrapper.h"
#include "TraceReader.h"

extern "C" {

    //MsgID
    MsgID* newMsgID(int size, int pe, int id){return new MsgID(size, pe, id);}
    int MsgID_getSize(MsgID* m){return m->size;}
    int MsgID_getID(MsgID* m){return m->pe;}
    int MsgID_getPE(MsgID* m){return m->id;}

    //MsgEntry
    MsgEntry* newMsgEntry(){return new MsgEntry();}
    int MsgEntry_getSize(MsgEntry* m){return m->msgId.size;}
    int MsgEntry_getPE(MsgEntry* m){return m->msgId.pe;}
    int MsgEntry_getID(MsgEntry* m){return m->msgId.id;}
    int MsgEntry_getNode(MsgEntry* m){return m->node;}
    int MsgEntry_getThread(MsgEntry* m){return m->thread;}
    unsigned long long MsgEntry_getSendOffset(MsgEntry* m){return m->sendOffset;}

    //PE
    PE* newPE(){return new PE();}
    void PE_set_busy(PE* p, bool b){p->busy = b;}
    bool PE_is_busy(PE* p){return p->busy;}
    bool PE_noUnsatDep(PE* p, int tInd){return p->noUnsatDep(tInd);}
    unsigned long long PE_getTaskExecTime(PE* p, int tInd){return p->noUnsatDep(tInd);}
    int PE_getTaskMsgEntryCount(PE* p, int tInd){return p->myTasks[tInd].msgEntCount;}
    MsgEntry** PE_getTaskMsgEntries(PE* p, int tInd){
        //printf("p->myTasks[tInd].myEntries[0]:%d\n", p->myTasks[tInd].myEntries[0].msgId.pe);
        return &(p->myTasks[tInd].myEntries);
    }
    MsgEntry* PE_getTaskMsgEntry(PE* p, int tInd, int mInd){
        return &(p->myTasks[tInd].myEntries[mInd]);
    }
    void PE_set_taskDone(PE* p, int tInd, bool b){p->myTasks[tInd].done=b;}
    bool PE_get_taskDone(PE* p, int tInd){return p->myTasks[tInd].done;}
    int* PE_getTaskFwdDep(PE* p, int tInd){return p->myTasks[tInd].forwardDep;}
    int PE_getTaskFwdDepSize(PE* p, int tInd){return p->myTasks[tInd].forwDepSize;}
    void PE_set_currentTask(PE* p, int tInd){p->currentTask=tInd;}
    int PE_get_currentTask(PE* p){return p->currentTask;}
    void PE_increment_currentTask(PE* p, int tInd){
        if(PE_get_currentTask(p) == tInd){
            PE_set_currentTask(p, tInd+1);
            // Search the following tasks until you see a not done task,
            // and increment the current task
            int i;
            for(i=tInd+1; i<PE_get_tasksCount(p); i++){
                if(PE_get_taskDone(p, i)){
                    PE_set_currentTask(p, i+1);
                }
                else return;
            }
        }
    }
    int PE_get_myEmPE(PE* p){return p->myEmPE;}
    int PE_get_myNum(PE* p){return p->myNum;}
    void PE_addToBuffer(PE* p, int task_id){p->msgBuffer.push_back(task_id);}
    void PE_removeFromBuffer(PE* p, int task_id){
        //if the task_id is in the buffer, remove it from the buffer
        for(int i=0; i<p->msgBuffer.size(); i++){
            if(p->msgBuffer[i] == task_id){
                p->msgBuffer.erase(p->msgBuffer.begin()+i);
                break;
            }
        }
    }
    int PE_getNextBuffedMsg(PE* p){
        if(p->msgBuffer.size()<=0) return -1;
        else{
            int id = p->msgBuffer.front();
            p->msgBuffer.erase(p->msgBuffer.begin());
            return id;
        }
    }
    int PE_findTaskFromMsg(PE* p, MsgID* msgId){
        return p->findTaskFromMsg(msgId);
    }
    void PE_invertMsgPe(PE* p, int tInd){
        p->invertMsgPe(tInd);
    }
    MsgID PE_getTaskMsgID(PE* p, int tInd){
        return p->myTasks[tInd].myMsgId;
    }
    int PE_get_tasksCount(PE* p){return p->tasksCount;}
    int PE_get_totalTasksCount(PE* p){return p->totalTasksCount;}
    //void PE_printStat(PE* p){p->printStat();}
    void PE_printStat(PE* p){p->check();}
    int PE_get_numWorkThreads(PE* p){return p->numWth;}

    //TraceReader
    TraceReader* newTraceReader(){return new TraceReader();}
    void TraceReader_readTrace(TraceReader* t, int* tot, int* numnodes, int*
    empes, int* nwth, PE* pe, int penum, unsigned long long* startTime){
         t->readTrace(tot, numnodes, empes, nwth, pe, penum, startTime);
    }
    int TraceReader_totalWorkerProcs(TraceReader* t){return t->totalWorkerProcs;}

}