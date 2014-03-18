/*
 * ibProcessors3D.h
 */

#ifndef IB_PROCESSORS_3D_H
#define IB_PROCESSORS_3D_H

namespace plb{

  template<typename T, template<typename U> class Descriptor>
  struct SetSingleSphere3D : public BoxProcessingFunctional3D_L<T,Descriptor> {
  public:
    SetSingleSphere3D(T *x_, T *v_, T *omega_, T r_, int id_, bool initVelFlag_)
      : x(x_),v(v_),omega(omega_),r(r_),id(id_), initVelFlag(initVelFlag_) {}
    SetSingleSphere3D(T *x_, T *v_, T *omega_, T r_, int id_)
      : x(x_),v(v_),omega(omega_),r(r_),id(id_), initVelFlag(false) {}
    SetSingleSphere3D(T *x_, T *v_, T r_, int id_)
      : x(x_),v(v_),omega(0),r(r_),id(id_), initVelFlag(false) {}
    SetSingleSphere3D(const SetSingleSphere3D &orig)
      : x(orig.x),v(orig.v),omega(orig.omega),r(orig.r),id(orig.id) {}

    Box3D getBoundingBox() const { return Box3D(x[0]-r-2,x[0]+r+2,
                                                x[1]-r-2,x[1]+r+2,
                                                x[2]-r-2,x[2]+r+2);}

    virtual void process(Box3D domain, BlockLattice3D<T,Descriptor> &lattice);
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    SetSingleSphere3D<T,Descriptor>* clone() const;
  private:
    T *x,*v,*omega,r;
    int id;
    bool initVelFlag;
    T calcSolidFraction(T dx_, T dy_, T dz_, T r_);
    void setValues(Cell<T,Descriptor>& c, T sf, T dx, T dy, T dz);
    void setToZero(Cell<T,Descriptor>& c);
  };
  
  template<typename T, template<typename U> class Descriptor>
  void setSpheresOnLattice(MultiBlockLattice3D<T,Descriptor> &lattice,
                           T **x, T **v, T **omega, T *r, int **id, plint nSpheres, bool initVelFlag);

  
  template<typename T, template<typename U> class Descriptor>
  struct SumForceTorque3D : public ReductiveBoxProcessingFunctional3D_L<T,Descriptor> {
  public:
    SumForceTorque3D(plint nPart_, T **x_);
    SumForceTorque3D(SumForceTorque3D<T,Descriptor> const &orig)
      : ReductiveBoxProcessingFunctional3D_L<T,Descriptor>(orig),
        sumId(orig.sumId), x(orig.x), partId(orig.partId) {}

    virtual void process(Box3D domain, BlockLattice3D<T,Descriptor>& lattice);

    SumForceTorque3D<T,Descriptor>* clone() const;
    void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    std::vector<double>& getForceTorque(){return this->getStatistics().getSumVect();}

    double getForce(plint partId, plint coord);
    double getTorque(plint partId, plint coord);

  private:
    std::vector<plint> sumId;
    T **x;
    plint const *partId;
    void addForce(plint partId, plint coord, T value);
    void addTorque(plint partId, plint coord, T value);
  };

}; // plb

#include "ibProcessors3D.hh"

#endif // IB_PROCESSORS_3D_H
