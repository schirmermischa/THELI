#ifndef WAVELET2S_H
#define WAVELET2S_H
#include <vector>
#include <QString>
#include <complex>

using namespace std;

// 1D Functions
void* dwt1(QString, vector<double> &, vector<double> &, vector<double> &);
void* dyadic_zpad_1d(vector<double> &);
// double convol(vector<double> &, vector<double> &, vector<double> &);
int filtcoef(QString, vector<double> &, vector<double> &, vector<double> &, vector<double> &);
void downsamp(vector<double> &, int , vector<double> &);
void upsamp(const vector<double> &, int, vector<double> &);
void circshift(vector<double> &, int );
int sign(int);
void* idwt1(QString name, vector<double> &, vector<double> &, vector<double> &);
int vecsum(vector<double> &, vector<double> &, vector<double> &);

// 1D Symmetric Extension DWT Functions
void* dwt_sym(vector<double> &, int ,QString, vector<double> &, vector<double> &, vector<int> &);
void* dwt1_sym(const QString, vector<double> &, vector<double> &, vector<double> &);
void* idwt_sym(vector<double> &, vector<double> &, QString, vector<double> &, vector<int> &);
void* symm_ext(vector<double> &, int );
void* idwt1_sym(QString, vector<double> &, vector<double> &, vector<double> &); // Not Tested

// 1D Stationary Wavelet Transform
void* swt(vector<double> &, int , QString, vector<double> &, int &) ;
void* iswt(vector<double> &,int , QString, vector<double> &);
void* per_ext(vector<double> &, int );

// 2D Functions
void* branch_dn(QString, vector<double> &, vector<double> &, QString mode);
void* branch_up(QString, vector<double> &, vector<double> &, vector<double> &);
void* dyadic_zpad_2d(vector<vector<double> > &, vector<vector<double> > &);
void* dwt_output_dim(vector<vector<double> >&, int &, int & );
void* zero_remove(vector<vector<double> > &, vector<vector<double> > &) ;
void* getcoeff2d(vector<vector<double> > &, vector<vector<double> > &,
                 vector<vector<double> > &, vector<vector<double> > &, vector<double> &, int &);
void* idwt2(QString, vector<vector<double> > &, vector<vector<double> >  &,
            vector<vector<double> >  &, vector<vector<double> >  &, vector<vector<double> > &);
void idwt2_subroutine_A(QString name, const vector<vector<double> > sigInL, const vector<vector<double> > sigInH,
                        vector<vector<double> > sigOut, const long index1, const long index2);
void idwt2_subroutine_B(QString name, vector<vector<double> > sigInL, vector<vector<double> > sigInH,
                        vector<vector<double> > sigOut, const long index1, const long index2);
void* dwt2(QString, vector<vector<double> > &, vector<vector<double> >  &,
           vector<vector<double> >  &, vector<vector<double> > &, vector<vector<double> > &);
void dwt2_subroutine_A(const QString name, const vector<vector<double> > &signalIn,
                       vector<vector<double> > &signalOutL, vector<vector<double> > &signalOutH, long index1, long index2, int lf, QString mode);
void dwt2_subroutine_B(const QString name, const vector<vector<double> > &signalIn,
                       vector<vector<double> > &signalOutL, vector<vector<double> > &signalOutH, long index1, long index2, int lf, QString mode);
void* downsamp2(vector<vector<double> > &, vector<vector<double> > &, int, int);
void* upsamp2(vector<vector<double> > &, vector<vector<double> > &, int, int);

// 2D DWT (Symmetric Extension) Functions
void* dwt_2d_sym(vector<vector<double> > &, int , QString, vector<double> &, vector<double> & , vector<int> &, vector<long> &boundaries);
void* dwt2_sym(QString, vector<vector<double> > &, vector<vector<double> >  &,
               vector<vector<double> >  &, vector<vector<double> >  &, vector<vector<double> > &);
void* idwt_2d_sym(vector<double>  &, vector<double> &, QString, vector<vector<double> > &, vector<int> &);
void* circshift2d(vector<vector<double> > &, int , int );
void symm_ext2d(vector<vector<double> > &, vector<vector<double> > &, int );
void* dispDWT(vector<double> &, vector<vector<double> > &, vector<int> &, vector<int> &, int ) ;
void* dwt_output_dim_sym(vector<int> &, vector<int> &, int );

//2D Stationary Wavelet Transform
void* swt_2d(vector<vector<double> > &,int , QString, vector<double> &, vector<int> &length, vector<long> &boundaries);
void* per_ext2d(vector<vector<double> > &, vector<vector<double> > &, int );
void* iswt_2d(vector<double> &swtop, int J, QString nm, vector<vector<double> > &sig, int row, int col);
void swt_subroutine_A(const vector<vector<double>> signalIn, const vector<double> low_pass, const vector<double> high_pass,
                      vector<vector<double>> &signalOutL, vector<vector<double>> &signalOutH,
                      long index1, long index2, int lf);
void swt_subroutine_B(const vector<vector<double>> signalIn, const vector<double> low_pass, const vector<double> high_pass,
                      vector<vector<double>> &signalOutL, vector<vector<double>> &signalOutH,
                      long index1, long index2, long index3, int lf);


// FFT functions
double convfft(const vector<double> &, const vector<double> &, vector<double> &);
double convfftm(const vector<double> &, const vector<double> &, vector<double> &);
void* fft(vector<complex<double> > &, int, unsigned int);
void* bitreverse(vector<complex<double> > &);
//void* freq(vector<double> &, vector<double> &);

// New
void* dwt1_sym_m(const QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD);
void* idwt1_sym_m(QString name, vector<double> &X, vector<double> &app, vector<double> &detail);
void* dwt(vector<double> &sig, int J, QString nm, vector<double> &dwt_output, vector<double> &flag, vector<int> &length );
void* idwt(vector<double> &, vector<double> &, QString, vector<double> &, vector<int> &);
void* dwt_2d(vector<vector<double> > &, int, QString, vector<double> &, vector<double> & , vector<int> &, vector<long> &boundaries);
void* dwt1_m(QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD) ;
void* idwt_2d(vector<double>  &dwtop, vector<double> &flag, QString nm, vector<vector<double> > &idwt_output, vector<int> &length);
void* idwt1_m(QString name, vector<double> &X, const vector<double> &cA, const vector<double> &cD);
void* dwt_output_dim2(vector<int> &length, vector<int> &length2, int J);
void* idwt_2d_old(vector<double>  &dwtop,vector<double> &flag, QString nm,
          vector<vector<double> > &idwt_output, vector<int> &length);


#endif/* WAVELET2S_H */
