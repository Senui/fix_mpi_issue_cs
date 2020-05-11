#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "mpi.h"
#include <TMessage.h>

using std::vector;

struct XMLParams : public TMessage
{
    template <typename T>
    void Append(vector<T> vec)
    {
        vector<double> v(vec.size());
        data_.push_back(v);
    }

    vector<vector<double>> GetData() const { return data_; }
    void SetData(int i, int j, double val) { data_[i][j] = val; }

    private:
    vector<vector<double>> data_;
    ClassDef(XMLParams, 1);
};

inline std::ostream& operator<<(std::ostream& os, const XMLParams& vv)
{
    const auto& vec = vv.GetData();
    os << "{";
    for (auto v = vec.begin(); v != vec.end(); ++v)
    {
        os << "[";
        for (auto ii = v->begin(); ii != v->end(); ++ii)
        {
            os << *ii;
            if (ii != (--v->end()))
                os << ", ";
        }
        os << "]";
        if (v != (--vec.end()))
            os << ", ";
    }
    os << "}";
    return os;
}

class MPIObject : public TMessage
{
    public:
    MPIObject() = default;
    MPIObject(void* buf, Int_t len) : TMessage(buf, len) {}
};

/// Send object to worker using ROOT Serialization
template <typename T>
int MPI_Send_Obj_ROOT(T* obj, int dest, int tag, MPI_Status* status = MPI_STATUS_IGNORE)
{
    
    MPIObject mpio;
    mpio.WriteObject(obj);
    int size = mpio.Length();
    // First send the size of the buffer
    MPI_Send(&size, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    // Then send the buffer
    return MPI_Send(mpio.Buffer(), size, MPI_BYTE, dest, tag, MPI_COMM_WORLD);
    
}

/// Receive object from master using ROOT Serialization
template <typename T>
T* MPI_Recv_Obj_ROOT(int size, int source, int tag, MPI_Status* status = MPI_STATUS_IGNORE)
{
    char* buf = (char*)malloc(size);
    // Then receive the buffer
    MPI_Recv(buf, size, MPI_BYTE, source, tag, MPI_COMM_WORLD, status);
    MPIObject* mpio = new MPIObject(buf, size);
    T* obj = (T*)(mpio->ReadObject(mpio->GetClass()));
    free(buf);
    return obj;
}