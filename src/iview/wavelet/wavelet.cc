//============================================================================
// Name        : 1D/2D Wavelet Transform
// Author      : Rafat Hussain; Mischa Schirmer (C++ clean-up and rewrite including parallelization)
// Version     :
// Copyright   : GNU Lesser GPL License
// Description : Wavelet Library
//============================================================================

#include <iostream>
#include <fstream>
#include <complex>
#include "wavelet.h"
#include <vector>
#include <QString>
#include <QDebug>
#include <cmath>
// #include <algorithm>
#include "fftw3.h"
#include <omp.h>

// using namespace std;
fftw_plan plan_forward_inp, plan_forward_filt, plan_backward;
static unsigned int transient_size_of_fft = 0;

void* per_ext2d(vector<vector<double> > &signal, vector<vector<double> > &temp2, int a)
{
    unsigned int rows = signal.size();
    unsigned int cols = signal[0].size();
    int cols2;
    if ((cols % 2) != 0) cols2 = cols + 1;
    else cols2 = cols;

    vector<vector<double> > temp_vec(rows, vector<double>(cols2 + 2*a));

    for (unsigned int i=0; i<rows; ++i) {
        vector<double> sig = signal[i];
        per_ext(sig, a);
        temp_vec[i] = sig;
    }
    long temp_vec0_size = temp_vec[0].size();
    for (int j=0; j<temp_vec0_size; ++j) {
        vector<double> sig;
        sig.reserve(rows);
        for (unsigned int i=0; i<rows; ++i) {
            sig.push_back(temp_vec[i][j]);
        }
        per_ext(sig, a);
        long sig_size = sig.size();
        for (int i=0; i<sig_size; ++i) {
            temp2[i][j] = sig[i];
        }
    }
    return 0;
}

void* swt_2d(vector<vector<double> > &sig, int J, QString name, vector<double> &swt_output, vector<int> &length, vector<long> &boundaries)
{
    int m_size = sig.size();
    int n_size = sig[0].size();

    int rows_n = m_size;
    int cols_n = n_size;
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    vector<double> low_pass = lp1;
    vector<double> high_pass = hp1;

    // The length vector is only used in tin swt_2d() so that we can more easily exchange
    // dwt_2d() and swt_2d() in top level code
    length.insert(length.begin(), cols_n);
    length.insert(length.begin(), rows_n);

    long breakPoint = 0;

    for (int iter=0; iter<J; ++iter) {
        length.insert(length.begin(), cols_n);
        length.insert(length.begin(), rows_n);

        int U = pow(2, iter);
        if (iter > 0) {
            upsamp(lp1, U, low_pass);
            upsamp(hp1, U, high_pass);
        }
        int lf = low_pass.size();

        if ((sig.size() % 2) == 0 ) rows_n = sig.size();
        else rows_n = sig.size() + 1;

        if ((sig[0].size() % 2) == 0 ) cols_n = sig[0].size();
        else cols_n = sig[0].size() + 1;

        vector<vector<double> > signal(rows_n+lf, vector<double>(cols_n+lf));

        per_ext2d(sig, signal, lf/2);

        int len_x = signal.size();
        vector<vector<double> > sigL(rows_n+lf, vector<double>(cols_n));
        vector<vector<double> > sigH(rows_n+lf, vector<double>(cols_n));
        vector<vector<double> > cA(rows_n, vector<double>(cols_n));
        vector<vector<double> > cH(rows_n, vector<double>(cols_n));
        vector<vector<double> > cV(rows_n, vector<double>(cols_n));
        vector<vector<double> > cD(rows_n, vector<double>(cols_n));

        swt_subroutine_A(signal, low_pass, high_pass, sigL, sigH, len_x, cols_n, lf);
        swt_subroutine_B(sigL, low_pass, high_pass, cA, cH, cols_n, len_x, rows_n, lf);
        swt_subroutine_B(sigH, low_pass, high_pass, cV, cD, cols_n, len_x, rows_n, lf);
        sig = cA;

        vector<double> temp_sig2;
        temp_sig2.reserve(4*rows_n*cols_n);

        if (iter == J-1) {
            for (int i=0; i<rows_n; ++i) {
                temp_sig2.insert(end(temp_sig2), begin(cA[i]), end(cA[i]));
            }
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cH[i]), end(cH[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cV[i]), end(cV[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cD[i]), end(cD[i]));
        }

        swt_output.insert(swt_output.begin(), temp_sig2.begin(), temp_sig2.end());

        breakPoint += temp_sig2.size();
        boundaries.push_back(breakPoint);

        // Qt5 rewrite
        //        for (auto &it: temp_sig2) {
        //            swt_output.insert(swt_output.begin(), it);
        //        }
    }

    // Re-order boundaries so that we can identify the coefficients of the various transforms
    reverse(boundaries.begin(), boundaries.end());
    long dim = boundaries[0];
    for (auto &it : boundaries) {
        it = dim - it;
    }
    boundaries.push_back(dim);

    return 0;
}

void swt_subroutine_A(const vector<vector<double>> signalIn, const vector<double> low_pass, const vector<double> high_pass,
                      vector<vector<double>> &signalOutL, vector<vector<double>> &signalOutH, long index1, long index2, int lf)
{
    for (long i=0; i<index1; ++i) {
        vector<double> temp_row = signalIn[i];

        // Low Pass Branch
        vector<double> oupL;
        convfftm(temp_row, low_pass, oupL);
        oupL.erase(oupL.begin(), oupL.begin()+lf);
        oupL.erase(oupL.begin()+index2, oupL.end());

        // High Pass Branch
        vector<double> oupH;
        convfftm(temp_row, high_pass, oupH);
        oupH.erase(oupH.begin(), oupH.begin()+lf);
        oupH.erase(oupH.begin()+index2, oupH.end());

        signalOutL[i] = oupL;
        signalOutH[i] = oupH;
    }
}

void swt_subroutine_B(const vector<vector<double>> signalIn, const vector<double> low_pass, const vector<double> high_pass,
                      vector<vector<double>> &signalOutL, vector<vector<double>> &signalOutH,
                      long index1, long index2, long index3, int lf)
{
    for (long j=0; j<index1; ++j) {
        vector<double> temp_row(index2);
        for (long i=0; i<index2; ++i) {
            temp_row.push_back(signalIn[i][j]);
        }

        // Low Pass Branch
        vector<double> oupL;
        convfftm(temp_row, low_pass, oupL);
        oupL.erase(oupL.begin(), oupL.begin()+lf);
        oupL.erase(oupL.begin()+index3, oupL.end());

        // High Pass Branch
        vector<double> oupH;
        convfftm(temp_row, high_pass, oupH);
        oupH.erase(oupH.begin(), oupH.begin()+lf);
        oupH.erase(oupH.begin()+index3, oupH.end());

        long oup_size = oupL.size();
        for (long i=0; i<oup_size; ++i) {
            signalOutL[i][j] = oupL[i];
            signalOutH[i][j] = oupH[i];
        }
    }
}

void* iswt_2d(vector<double> &swtop, int J, QString name, vector<vector<double> > &sig, int row, int col)
{
    // swtop [1D Output vector from swt_2d]
    // J [Number of Levels]
    // sig [Output]
    // row & col [Rows and cols of sig]

    // 1. Nason, G.P. and Silverman, B.W. (1995) The stationary wavelet transform and some statistical applications.
    // In Antoniadis, A. and Oppenheim, G. (eds) Lecture Notes in Statistics, 103, 281--300.

    // 2. Pesquet, J.C.; H. Krim, H. Carfatan (1996), "Time-invariant orthonormal wavelet representations,"
    // http://pdf.aminer.org/000/318/848/frequency_shift_invariant_orthonormal_wavelet_packet_representations.pdf

    int rows_n = row;
    int cols_n = col;
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    vector<double> low_pass = lp2;
    vector<double> high_pass = hp2;
    int lf = low_pass.size();
    int sum = 0;

    vector<vector<double> > cLL(rows_n, vector<double>(cols_n));

    for (int iter=J; iter>0; iter--) {
        vector<vector<double> > cLH(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHL(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHH(rows_n, vector<double>(cols_n));

        if (iter == J) {
            for (int i=0; i<rows_n; ++i) {
                for (int j=0; j<cols_n; ++j) {
                    cLL[i][j] = swtop[sum + i*cols_n + j];
                    cHL[i][j] = swtop[sum + 1*rows_n*cols_n + i*cols_n + j];
                    cLH[i][j] = swtop[sum + 2*rows_n*cols_n + i*cols_n + j];
                    cHH[i][j] = swtop[sum + 3*rows_n*cols_n + i*cols_n + j];
                }
            }
            sum += 4*rows_n*cols_n;
        }
        else {
            for (int i=0; i<rows_n; ++i) {
                for (int j = 0; j<cols_n; ++j) {
                    cHL[i][j] = swtop[sum + i*cols_n+j];
                    cLH[i][j] = swtop[sum + 1*rows_n*cols_n + i*cols_n + j];
                    cHH[i][j] = swtop[sum + 2*rows_n*cols_n + i*cols_n + j];
                }
            }
            sum += 3*rows_n*cols_n;
        }

        int value = pow(2.0, iter-1);

        for (int count=0; count<value; ++count) {
            vector<vector<double> > cLL1(rows_n/value, vector<double>(cols_n/value));
            vector<vector<double> > cLH1(rows_n/value, vector<double>(cols_n/value));
            vector<vector<double> > cHL1(rows_n/value, vector<double>(cols_n/value));
            vector<vector<double> > cHH1(rows_n/value, vector<double>(cols_n/value));
            int row1 = 0;
            int col1 = 0;
            for (int index_r=count; index_r<rows_n; index_r+=value) {
                for (int index_c=count; index_c<cols_n; index_c+=value) {
                    cLL1[row1][col1] = cLL[index_r][index_c];
                    cLH1[row1][col1] = cLH[index_r][index_c];
                    cHL1[row1][col1] = cHL[index_r][index_c];
                    cHH1[row1][col1] = cHH[index_r][index_c];
                    ++col1;
                }
                col1 = 0;
                ++row1;
            }

            // Shift = 0
            int len_c = cLL1[0].size();
            int len_r = cLL1.size();

            vector<vector<double> > cLL2(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cLH2(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cHL2(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cHH2(len_r/2, vector<double>(len_c/2));
            row1 = 0;
            col1 = 0;
            for (int index_r=0; index_r<len_r; index_r+=2) {
                for (int index_c=0; index_c<len_c; index_c+=2) {
                    cLL2[row1][col1] = cLL1[index_r][index_c];
                    cLH2[row1][col1] = cLH1[index_r][index_c];
                    cHL2[row1][col1] = cHL1[index_r][index_c];
                    cHH2[row1][col1] = cHH1[index_r][index_c];
                    ++col1;
                }
                col1 = 0;
                ++row1;
            }

            vector<vector<double> > cLLu(len_r, vector<double>(len_c));
            vector<vector<double> > cLHu(len_r, vector<double>(len_c));
            vector<vector<double> > cHLu(len_r, vector<double>(len_c));
            vector<vector<double> > cHHu(len_r, vector<double>(len_c));
            upsamp2(cLL2, cLLu, 2, 2);
            upsamp2(cLH2, cLHu, 2, 2);
            upsamp2(cHL2, cHLu, 2, 2);
            upsamp2(cHH2, cHHu, 2, 2);

            vector<vector<double> > cLL00(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cLH00(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cHL00(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cHH00(len_r+lf, vector<double>(len_c + lf));
            per_ext2d(cLLu, cLL00, lf/2);
            per_ext2d(cLHu, cLH00, lf/2);
            per_ext2d(cHLu, cHL00, lf/2);
            per_ext2d(cHHu, cHH00, lf/2);

            // Shift by 1
            vector<vector<double> > cLL3(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cLH3(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cHL3(len_r/2, vector<double>(len_c/2));
            vector<vector<double> > cHH3(len_r/2, vector<double>(len_c/2));
            col1 = 0;
            row1 = 0;

            for (int index_r=1; index_r<len_r; index_r+=2) {
                for (int index_c=1; index_c<len_c; index_c+=2) {
                    cLL3[row1][col1] = cLL1[index_r][index_c];
                    cLH3[row1][col1] = cLH1[index_r][index_c];
                    cHL3[row1][col1] = cHL1[index_r][index_c];
                    cHH3[row1][col1] = cHH1[index_r][index_c];
                    ++col1;
                }
                col1 = 0;
                ++row1;
            }

            vector<vector<double> > cLLu2(len_r, vector<double>(len_c));
            vector<vector<double> > cLHu2(len_r, vector<double>(len_c));
            vector<vector<double> > cHLu2(len_r, vector<double>(len_c));
            vector<vector<double> > cHHu2(len_r, vector<double>(len_c));

            upsamp2(cLL3, cLLu2, 2, 2);
            upsamp2(cLH3, cLHu2, 2, 2);
            upsamp2(cHL3, cHLu2, 2, 2);
            upsamp2(cHH3, cHHu2, 2, 2);

            vector<vector<double> > cLL01(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cLH01(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cHL01(len_r+lf, vector<double>(len_c + lf));
            vector<vector<double> > cHH01(len_r+lf, vector<double>(len_c + lf));
            per_ext2d(cLLu2, cLL01, lf/2);
            per_ext2d(cLHu2, cLH01, lf/2);
            per_ext2d(cHLu2, cHL01, lf/2);
            per_ext2d(cHHu2, cHH01, lf/2);

            int rowsLL = cLL01.size();
            int colsLL = cLL01[0].size();

            vector<vector<double> > cLLt00(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cLHt00(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cHLt00(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cHHt00(rowsLL + lf-1, vector<double>(colsLL));

            vector<vector<double> > cLLt10(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cLHt10(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cHLt10(rowsLL + lf-1, vector<double>(colsLL));
            vector<vector<double> > cHHt10(rowsLL + lf-1, vector<double>(colsLL));

            vector<vector<double> > cLLt01(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cLHt01(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cHLt01(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cHHt01(rowsLL + lf-1, vector<double>(colsLL + lf-1));

            vector<vector<double> > cLLt11(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cLHt11(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cHLt11(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cHHt11(rowsLL + lf-1, vector<double>(colsLL + lf-1));

            vector<vector<double> > cLLt0(rowsLL + lf-1, vector<double>(colsLL + lf-1));
            vector<vector<double> > cLLt1(rowsLL + lf-1, vector<double>(colsLL + lf-1));

            vector<vector<double> > oupt0(len_r, vector<double>(len_c));
            vector<vector<double> > oupt1(len_r, vector<double>(len_c));
            vector<vector<double> > oupt(len_r, vector<double>(len_c));

            for (int j=0; j<colsLL; ++j) {
                vector<double> sigLL0;
                vector<double> sigLH0;
                vector<double> sigHL0;
                vector<double> sigHH0;
                sigLL0.reserve(rowsLL);
                sigLH0.reserve(rowsLL);
                sigHL0.reserve(rowsLL);
                sigHH0.reserve(rowsLL);
                for (int i=0; i<rowsLL; ++i) {
                    sigLL0.push_back(cLL00[i][j]);
                    sigLH0.push_back(cLH00[i][j]);
                    sigHL0.push_back(cHL00[i][j]);
                    sigHH0.push_back(cHH00[i][j]);
                }

                // First Convolution Step for LL, LH, HL, HH
                vector<double> X0LL;
                vector<double> X0LH;
                vector<double> X0HL;
                vector<double> X0HH;
                convfft(sigLL0, low_pass, X0LL);
                convfft(sigLH0, low_pass, X0LH);
                convfft(sigHL0, high_pass, X0HL);
                convfft(sigHH0, high_pass, X0HH);

                int lent = X0LL.size();
                for (int i=0; i<lent; ++i) {
                    cLLt00[i][j] = X0LL[i];
                    cLHt00[i][j] = X0LH[i];
                    cHLt00[i][j] = X0HL[i];
                    cHHt00[i][j] = X0HH[i];
                }
            }

            for (int i=0; i<rowsLL+lf-1; ++i) {
                vector<double> sigLL0 = cLLt00[i];
                vector<double> sigLH0 = cLHt00[i];
                vector<double> sigHL0 = cHLt00[i];
                vector<double> sigHH0 = cHHt00[i];

                // Second Convolution Step for LL, LH, HL, HH
                vector<double> X0LL;
                vector<double> X0LH;
                vector<double> X0HL;
                vector<double> X0HH;
                // WARNING: check low_pass high_pass arguments! different order than in code-block above!!
                convfft(sigLL0, low_pass, X0LL);
                convfft(sigLH0, high_pass, X0LH);
                convfft(sigHL0, low_pass, X0HL);
                convfft(sigHH0, high_pass, X0HH);

                cLLt01[i] = X0LL;
                cLHt01[i] = X0LH;
                cHLt01[i] = X0HL;
                cHHt01[i] = X0HH;
                int lent = X0LL.size();
                for (int j=0; j<lent; ++j) {
                    cLLt0[i][j] = X0LL[j] + X0LH[j] + X0HL[j] + X0HH[j];
                }
            }

            for (int j=0; j<colsLL; ++j) {
                vector<double> sigLL0;
                vector<double> sigLH0;
                vector<double> sigHL0;
                vector<double> sigHH0;
                sigLL0.reserve(rowsLL);
                sigLH0.reserve(rowsLL);
                sigHL0.reserve(rowsLL);
                sigHH0.reserve(rowsLL);
                for (int i=0; i<rowsLL; ++i) {
                    sigLL0.push_back(cLL01[i][j]);
                    sigLH0.push_back(cLH01[i][j]);
                    sigHL0.push_back(cHL01[i][j]);
                    sigHH0.push_back(cHH01[i][j]);
                }

                // First Convolution Step for LL, LH, HL, HH
                vector<double> X0LL;
                vector<double> X0LH;
                vector<double> X0HL;
                vector<double> X0HH;
                convfft(sigLL0, low_pass, X0LL);
                convfft(sigLH0, low_pass, X0LH);
                convfft(sigHL0, high_pass, X0HL);
                convfft(sigHH0, high_pass, X0HH);

                int lent = X0LL.size();
                for (int i=0; i<lent; ++i) {
                    cLLt10[i][j] = X0LL[i];
                    cLHt10[i][j] = X0LH[i];
                    cHLt10[i][j] = X0HL[i];
                    cHHt10[i][j] = X0HH[i];
                }
            }

            for (int i=0; i<rowsLL+lf-1; ++i) {
                vector<double> sigLL0 = cLLt10[i];
                vector<double> sigLH0 = cLHt10[i];
                vector<double> sigHL0 = cHLt10[i];
                vector<double> sigHH0 = cHHt10[i];

                // Second Convolution Step for LL,LH,HL,HH
                vector<double> X0LL;
                vector<double> X0LH;
                vector<double> X0HL;
                vector<double> X0HH;
                convfft(sigLL0, low_pass, X0LL);
                convfft(sigLH0, high_pass, X0LH);
                convfft(sigHL0, low_pass, X0HL);
                convfft(sigHH0, high_pass, X0HH);

                cLLt11[i] = X0LL;
                cLHt11[i] = X0LH;
                cHLt11[i] = X0HL;
                cHHt11[i] = X0HH;
                int lent = X0LL.size();
                for (int j=0; j<lent; ++j) {
                    cLLt1[i][j] = X0LL[j] + X0LH[j] + X0HL[j] + X0HH[j];
                }
            }

            for (int i=0; i<len_r; ++i) {
                for (int j=0; j<len_c; ++j) {
                    oupt0[i][j] = cLLt0[lf+i-1][lf+j-1];
                    oupt1[i][j] = cLLt1[lf+i-1][lf+j-1];
                }
            }

            circshift2d(oupt1,-1,-1);

            for (int i=0; i<len_r; ++i) {
                for (int j=0; j<len_c; ++j) {
                    oupt[i][j] = (oupt0[i][j] + oupt1[i][j]) / 2.0;
                }
            }

            row1 = 0;
            col1 = 0;

            for (int index_r=count; index_r<rows_n; index_r+=value) {
                for (int index_c=count; index_c<cols_n; index_c+=value) {
                    cLL[index_r][index_c] = oupt[row1][col1];
                    ++col1;
                }
                col1 = 0;
                ++row1;
            }
        }

        if (iter == 1) {
            sig = cLL;
        }
    }

    return 0;
}

void* per_ext(vector<double> &sig, int a)
{
    unsigned int len = sig.size();
    if ((len % 2) != 0 ) {
        sig.push_back(sig[len-1]);
        len = sig.size();
    }

    // TODO: insert at the beginning is extremely inefficient. Probably doesn't matter too much here
    for (int i=0; i<a; ++i) {
        sig.insert(sig.begin(), sig[len-1]);
        sig.insert(sig.end(), sig[2*i]);
    }

    return 0;
}

void* iswt(vector<double> &swtop, int J, QString name, vector<double> &iswt_output)
{
    int N = swtop.size() / (J+1);
    int U = 2; // Upsampling Factor

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    vector<double> appx_sig;
    vector<double> low_pass = lp2;
    vector<double> high_pass = hp2;
    int lf = low_pass.size();

    for (int iter=0; iter<J; ++iter) {
        vector<double> det_sig(N);
        if (iter == 0) {
            for (int i=0; i<N; ++i) {
                appx_sig.push_back(swtop[i]);
            }
        }

        for (int i=0; i<N; ++i) {
            det_sig.push_back(swtop[(iter+1)*N+i]);
        }

        int value = pow(2, J-1-iter);
        iswt_output.assign(N,0.0);

        for (int count=0; count<value; count++) {
            int dim = (N-1-count) / value + 1;
            // Vector size assignments are just to avoid repeated memory allocations. Should be accurate, or within +/-1.
            // push_back() will request more space if necessary.
            vector<double> appx1(dim);
            vector<double> det1(dim);
            for (int index=count; index<N; index+=value) {
                appx1.push_back(appx_sig[index]);
                det1.push_back(det_sig[index]);
            }
            int len = appx1.size();

            // Shift = 0
            vector<double> appx2(len/2);
            vector<double> det2(len/2);
            for (int index_shift=0; index_shift<len; index_shift+=2) {
                appx2.push_back(appx1[index_shift]);
                det2.push_back(det1[index_shift]);
            }

            vector<double> cL0, cH0;
            upsamp(appx2, U, cL0);
            upsamp(det2, U, cH0);
            per_ext(cL0, lf/2);
            per_ext(cH0, lf/2);
            vector<double> oup00L, oup00H;
            convfft(cL0, low_pass, oup00L);
            convfft(cH0, high_pass, oup00H);
            oup00L.erase(oup00L.begin(), oup00L.begin()+lf-1);
            oup00L.erase(oup00L.begin()+len, oup00L.end());
            oup00H.erase(oup00H.begin(), oup00H.begin()+lf-1);
            oup00H.erase(oup00H.begin()+len, oup00H.end());
            vector<double> oup00;
            vecsum(oup00L, oup00H, oup00);

            // Shift = 1
            vector<double> appx3((len-1)/2);
            vector<double> det3((len-1)/2);
            for (int index_shift=1; index_shift<len; index_shift+=2) {
                appx3.push_back(appx1[index_shift]);
                det3.push_back(det1[index_shift]);
            }

            vector<double> cL1, cH1;
            upsamp(appx3, U, cL1);
            upsamp(det3, U, cH1);
            per_ext(cL1, lf/2);
            per_ext(cH1, lf/2);
            vector<double> oup01L, oup01H;
            convfft(cL1, low_pass, oup01L);
            convfft(cH1, high_pass, oup01H);
            oup01L.erase(oup01L.begin(), oup01L.begin()+lf-1);
            oup01L.erase(oup01L.begin()+len, oup01L.end());
            oup01H.erase(oup01H.begin(), oup01H.begin()+lf-1);
            oup01H.erase(oup01H.begin()+len, oup01H.end());
            vector<double> oup01;
            vecsum(oup01L, oup01H, oup01);

            circshift(oup01, -1);

            //   Continue
            int index2 = 0;
            for (int index=count; index<N; index+=value) {
                iswt_output[index] = (oup00[index2] + oup01[index2]) / 2.;
                ++index2;
            }
        }
        appx_sig = iswt_output;
    }
    return 0;
}

void* swt(vector<double> &signal1, int J, QString name, vector<double> &swt_output, int &length)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    vector<double> sig = signal1;
    int N = sig.size();
    length = N;

    vector<double> low_pass = lp1;
    vector<double> high_pass = hp1;

    for (int iter=0; iter<J; ++iter) {
        if (iter>0) {
            int U = pow(2, iter);
            upsamp(lp1, U, low_pass);
            upsamp(hp1, U, high_pass);
        }
        int lf = low_pass.size();

        per_ext(sig, lf/2);

        vector<double> cA;
        convfft(sig, low_pass, cA);
        vector<double> cD;
        convfft(sig, high_pass, cD);
        // Resize cA and cD
        cA.erase(cA.begin(), cA.begin()+lf);
        cA.erase(cA.begin()+N, cA.end());
        cD.erase(cD.begin(), cD.begin()+lf);
        cD.erase(cD.begin()+N, cD.end());
        // Reset signal value;

        sig = cA;

        if (iter == J - 1 ) {
            swt_output.insert(swt_output.begin(), cD.begin(), cD.end());
            swt_output.insert(swt_output.begin(), cA.begin(), cA.end());
            // Qt5 rewrite
            //vector<double>::iterator itpos = swt_output.begin();
            //for (auto &it: cD) {
            //    swt_output.insert(itpos, it);
            //    ++itpos;
            //}
            //itpos = swt_output.begin();
            //for (auto &it: cA) {
            //    swt_output.insert(itpos, it);
            //    ++itpos;
            //}
        }
        else {
            swt_output.insert(swt_output.begin(), cD.begin(), cD.end());
            //vector<double>::iterator itpos = swt_output.begin();
            //for (auto &it: cD) {
            //    swt_output.insert(itpos, it);
            //    ++itpos;
            //}
        }
    }

    return 0;
}

void* dwt_output_dim_sym(vector<int> &length, vector<int> &length2, int J)
{
    unsigned int sz = length.size();
    int rows = length[sz-2];
    int cols = length[sz-1];
    for (int i=0; i<J; ++i) {
        rows = ceil((double) rows / 2.0);
        cols = ceil((double) cols / 2.0);
    }
    for (int i=0; i<J+1; ++i) {
        length2.push_back(rows);
        length2.push_back(cols);
        rows *= 2;
        cols *= 2;
    }
    return 0;
}

void* dwt_output_dim2(vector<int> &length, vector<int> &length2, int J)
{
    int row = length[0];
    int col = length[1];

    for (int i=0; i<J+1; ++i) {
        length2.push_back(row);
        length2.push_back(col);
        row *= 2;
        col *= 2;
    }
    return 0;
}

void* dispDWT(vector<double> &output, vector<vector<double> > &dwtdisp, vector<int> &length,
              vector<int> &length2, int J)
{
    int sum = 0;

    for (int iter=0; iter<J; ++iter) {
        int d_rows = length[2*iter] - length2[2*iter];
        int d_cols = length[2*iter+1] - length2[2*iter+1];

        int rows_n = length[2*iter];
        int cols_n = length[2*iter+1];
        vector<vector<double> > dwt_output(2*rows_n, vector<double>(2*cols_n));
        if (iter == 0) {
            for (int i=0; i<rows_n; ++i) {
                for (int j=0; j<cols_n; ++j) {
                    dwt_output[i][j] = output[i*cols_n+j];
                }
            }

            for (int i=0; i<rows_n; ++i) {
                for (int j=cols_n; j<cols_n*2; ++j) {
                    dwt_output[i][j] = output[rows_n*cols_n + i*cols_n + (j-cols_n)];
                }
            }

            for (int i=rows_n; i<rows_n*2; ++i) {
                for (int j=0; j<cols_n; ++j) {
                    dwt_output[i][j] = output[2*rows_n*cols_n + (i-rows_n)*cols_n + j];
                }
            }

            for (int i=rows_n; i<rows_n*2; ++i) {
                for (int j=cols_n; j<cols_n*2; ++j) {
                    dwt_output[i][j] = output[3*rows_n*cols_n + (i-rows_n)*cols_n + (j-cols_n)];
                }
            }
        }
        else {
            for (int i=0; i<rows_n; ++i) {
                for (int j=cols_n; j<cols_n*2; ++j) {
                    dwt_output[i][j] = output[sum + i*cols_n + (j-cols_n)];
                }
            }

            for (int i=rows_n; i<rows_n*2; ++i) {
                for (int j=0; j<cols_n; ++j) {
                    dwt_output[i][j] = output[sum + rows_n*cols_n + (i-rows_n)*cols_n + j];
                }
            }

            for (int i=rows_n; i<rows_n*2; ++i) {
                for (int j=cols_n; j<cols_n*2; ++j) {
                    dwt_output[i][j] = output[sum + 2*rows_n*cols_n + (i-rows_n)*cols_n + (j-cols_n)];
                }
            }
        }

        int rows_x = length2[2*iter];
        int cols_x =length2[2*iter+1];

        int d_cols2 = ceil( (double) (d_cols - 1) / 2.0);
        int d_rows2 = ceil( (double) (d_rows - 1) / 2.0);
        if (iter ==0) {
            for (int i=0; i<rows_x; ++i) {
                for (int j=0; j<cols_x; ++j) {
                    if (i+d_rows-1 < 0) {
                        dwtdisp[i][j] =0;
                    }
                    else if (j+d_cols-1 < 0) {
                        dwtdisp[i][j] = 0;
                    } else {
                        dwtdisp[i][j] = dwt_output[i+d_rows-1][j+d_cols-1];
                    }
                }
            }
        }
        for (int i=0; i<rows_x; ++i) {
            for (int j=cols_x; j<cols_x*2; ++j) {
                if (i+d_rows2 < 0) {
                    dwtdisp[i][j] =0;
                }
                else if (j+2*(d_cols-1)+1 > (signed) dwt_output[0].size() - 1) {
                    dwtdisp[i][j] = 0;
                } else {
                    dwtdisp[i][j] = dwt_output[i+d_rows2][j+2*(d_cols-1)+1];
                }
            }
        }

        for (int i=rows_x; i<rows_x*2; ++i) {
            for (int j=0; j<cols_x; ++j) {
                if (i+2*(d_rows-1)+1 > (signed) dwt_output.size() - 1) {
                    dwtdisp[i][j] = 0;
                }
                else if (j+d_cols2 < 0) {
                    dwtdisp[i][j] = 0;
                } else {
                    dwtdisp[i][j] = dwt_output[i+2*(d_rows-1)+1][j+d_cols2];
                }
            }
        }

        for (int i=rows_x; i<rows_x*2; ++i) {
            for (int j=cols_x; j<cols_x*2; ++j) {
                if (i+(d_rows-1)+1+d_rows2 > (signed) dwt_output.size() - 1) {
                    dwtdisp[i][j] = 0;
                }
                else if (j+(d_cols-1)+1+d_cols2 > (signed) dwt_output[0].size() - 1) {
                    dwtdisp[i][j] = 0;
                } else {
                    dwtdisp[i][j] = dwt_output[i+(d_rows-1)+1+d_rows2][j+(d_cols-1)+1+d_cols2 ];
                }
            }
        }
        if (iter == 0) sum += 4*rows_n*cols_n;
        else sum += 3*rows_n*cols_n;
    }

    return 0;

}

void symm_ext2d(vector<vector<double> > &signal, vector<vector<double> > &temp2, int a)
{
    long rows = signal.size();
    long cols = signal[0].size();
    vector<vector<double> > temp_vec(rows, vector<double>(cols + 2* a));

    for (long i=0; i<rows; ++i) {
        vector<double> sig = signal[i];
        symm_ext(sig, a);
        temp_vec[i] = sig;
    }

    long dim = temp_vec[0].size();
    for (long j=0; j<dim; ++j) {
        vector<double> sig;
        for (long i=0; i<rows; ++i) {
            sig.push_back(temp_vec[i][j]);
        }
        symm_ext(sig, a);
        long sig_size = sig.size();
        for (long i=0; i<sig_size; ++i) {
            temp2[i][j] = sig[i];
        }
    }
}

void* circshift2d(vector<vector<double> > &signal, int x, int y)
{
    int rows = signal.size();
    int cols = signal[0].size();
    vector<vector<double> > temp_vec(rows, vector<double>(cols));

    for (int i=0; i<rows; ++i) {
        vector<double> sig = signal[i];
        circshift(sig, x);
        temp_vec[i] = sig;
    }

    for (int j=0; j<cols; ++j) {
        vector<double> sig;
        for (int i=0; i<rows; ++i) {
            sig.push_back(temp_vec[i][j]);
        }
        circshift(sig, y);
        for (int i=0; i<rows; ++i) {
            signal[i][j] = sig[i];
        }
    }
    return 0;
}

void* idwt_2d_sym(vector<double> &dwtop, vector<double> &flag, QString name,
                  vector<vector<double> > &idwt_output, vector<int> &length)
{
    int J = (int) flag[0];
    int rows = length[0];
    int cols = length[1];
    int sum_coef = 0;
    long oup_size = 0;
    vector<vector<double> > cLL(rows, vector<double>(cols));

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int lf = lp1.size();

    for (int iter=0; iter<J; ++iter) {
        long rows_n = length[2*iter];
        long cols_n = length[2*iter+1];
        vector<vector<double> > cLH(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHL(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHH(rows_n, vector<double>(cols_n));

        for (long i=0; i<rows_n; ++i) {
            for (long j=0; j<cols_n; ++j) {
                if (iter == 0) {
                    cLL[i][j] = dwtop[sum_coef + i*cols_n + j];
                    cLH[i][j] = dwtop[sum_coef + i*cols_n + j + 1*rows_n*cols_n];
                    cHL[i][j] = dwtop[sum_coef + i*cols_n + j + 2*rows_n*cols_n];
                    cHH[i][j] = dwtop[sum_coef + i*cols_n + j + 3*rows_n*cols_n];
                }
                else {
                    cLH[i][j] = dwtop[sum_coef + i*cols_n + j];
                    cHL[i][j] = dwtop[sum_coef + i*cols_n + j + 1*rows_n*cols_n];
                    cHH[i][j] = dwtop[sum_coef + i*cols_n + j + 2*rows_n*cols_n];
                }
            }
        }

        long len_x = cLH.size();
        long len_y = cLH[0].size();

        // Row Upsampling and Column Filtering at the first LP Stage
        vector<vector<double> > cL(2*len_x-lf+2, vector<double>(len_y));
        vector<vector<double> > cH(2*len_x-lf+2, vector<double>(len_y));

        for (long j=0; j<len_y; ++j) {
            vector<double> sigLL;
            vector<double> sigLH;
            vector<double> sigHL;
            vector<double> sigHH;
            sigLL.reserve(len_x);
            sigLH.reserve(len_x);
            sigHL.reserve(len_x);
            sigHH.reserve(len_x);
            for (long i=0; i<len_x; ++i) {
                sigLL.push_back(cLL[i][j]);
                sigLH.push_back(cLH[i][j]);
                sigHL.push_back(cHL[i][j]);
                sigHH.push_back(cHH[i][j]);
            }
            vector<double> oupL;
            vector<double> oupH;
            oupL.reserve(len_x*2+lf);
            oupH.reserve(len_x*2+lf);
            idwt1_sym_m(name, oupL, sigLL, sigLH);
            idwt1_sym_m(name, oupH, sigHL, sigHH);
            oup_size = oupL.size();
            for (long i=0; i<oup_size; ++i) {
                cL[i][j] = oupL[i];
                cH[i][j] = oupH[i];
            }
        }

        vector<vector<double> > signal(2*len_x-lf+2, vector<double>(2*len_y-lf+2));
        for (long i=0; i<2*len_x-lf+2; ++i) {
            vector<double> sigL;
            vector<double> sigH;
            sigL.reserve(len_y);
            sigH.reserve(len_y);
            for (long j=0; j<len_y; ++j) {
                sigL.push_back(cL[i][j]);
                sigH.push_back(cH[i][j]);
            }
            vector<double> oup;
            oup.reserve(len_y*2+lf);
            idwt1_sym_m(name, oup, sigL, sigH);
            oup_size = oup.size();
            for (int j=0; j<oup_size; ++j) {
                signal[i][j] = oup[j];
            }
        }

        idwt_output = signal;

        if (iter == 0) sum_coef += 4*rows_n*cols_n;
        else sum_coef += 3*rows_n*cols_n;

        cLL = signal;
    }

    return 0;
}


void* dwt2_sym(QString name, vector<vector<double> > &signal, vector<vector<double> >  &cLL,
               vector<vector<double> >  &cLH, vector<vector<double> >  &cHL, vector<vector<double> > &cHH)
{
    int rows = signal.size();
    int cols = signal[0].size();
    int cols_lp1 = cLL[0].size();
    int cols_hp1 = cLL[0].size();
    vector<vector<double> > lp_dn1(rows, vector<double>(cols_lp1));
    vector<vector<double> > hp_dn1(rows, vector<double>(cols_hp1));

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int lf = lp1.size();

    // Implementing row filtering and column downsampling in each branch.
    dwt2_subroutine_A(name, signal, lp_dn1, hp_dn1, rows, cols, lf, "mode_sym");

    // Column filtering and row downsampling in Low/High Pass branches.
    dwt2_subroutine_B(name, lp_dn1, cLL, cLH, cols_lp1, rows, lf, "mode_sym");
    dwt2_subroutine_B(name, hp_dn1, cHL, cHH, cols_hp1, rows, lf, "mode_sym");

    return 0;
}

void dwt2_subroutine_A(const QString name, const vector<vector<double> > &signalIn,
                       vector<vector<double> > &signalOutL, vector<vector<double> > &signalOutH,
                       long index1, long index2, int lf, QString mode)
{
    for (long i=0; i<index1; ++i) {
        vector<double> temp_row = signalIn[i];
        vector<double> oupL;
        vector<double> oupH;
        oupL.reserve((index2+lf)/2.+1);
        oupH.reserve((index2+lf)/2.+1);
        if (mode == "mode_sym") dwt1_sym_m(name, temp_row, oupL, oupH);
        else dwt1_m(name, temp_row, oupL, oupH);
        signalOutL[i] = oupL;
        signalOutH[i] = oupH;
    }
}

void dwt2_subroutine_B(const QString name, const vector<vector<double> > &signalIn,
                       vector<vector<double> > &signalOutL, vector<vector<double> > &signalOutH,
                       long index1, long index2, int lf, QString mode)
{
    for (long j=0; j<index1; ++j) {
        vector<double> temp_row;
        temp_row.reserve(index2);
        for (int i=0; i<index2; ++i) {
            temp_row.push_back(signalIn[i][j]);
        }
        vector<double> oupL;
        vector<double> oupH;
        oupL.reserve((index1+lf)/2.+1);
        oupH.reserve((index1+lf)/2.+1);
        if (mode == "mode_sym") dwt1_sym_m(name, temp_row, oupL, oupH);
        else dwt1_m(name, temp_row, oupL, oupH);
        long oup_size = oupL.size();
        for (int i=0; i<oup_size; ++i) {
            signalOutL[i][j] = oupL[i];
            signalOutH[i][j] = oupH[i];
        }
    }
}

void* dwt_2d_sym(vector<vector<double> > &origsig, int J, QString name, vector<double> &dwt_output,
                 vector<double> &flag, vector<int> &length, vector<long> &boundaries)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int lf = lp1.size();

    vector<vector<double> > sig = origsig;
    int rows_n = sig.size();
    int cols_n = sig[0].size();
    vector<vector<double> > original_copy(rows_n, vector<double>(cols_n));

    original_copy = sig;
    int Max_Iter = min(ceil(log( double(sig.size()))/log (2.0)), ceil(log( double(sig[0].size()))/log (2.0)));
    if (Max_Iter< J) {
        qDebug() << J << " iterations are not possible with signals of this dimension.";
        exit(1);
    }
    flag.push_back(double(J));

    length.insert(length.begin(), cols_n);
    length.insert(length.begin(), rows_n);

    long breakPoint = 0;
    for (int iter=0; iter<J; ++iter) {
        rows_n = floor((double) (rows_n + lf - 1)/2);
        cols_n = floor((double) (cols_n + lf - 1)/2);
        length.insert(length.begin(), cols_n);
        length.insert(length.begin(), rows_n);

        vector<vector<double> > cA(rows_n, vector<double>(cols_n));
        vector<vector<double> > cH(rows_n, vector<double>(cols_n));
        vector<vector<double> > cV(rows_n, vector<double>(cols_n));
        vector<vector<double> > cD(rows_n, vector<double>(cols_n));

        dwt2_sym(name, original_copy, cA, cH, cV, cD);

        vector<double> temp_sig2;
        temp_sig2.reserve(4*rows_n*cols_n);

        original_copy = cA;
        if (iter == J-1) {
            for (int i=0; i<rows_n; ++i) {
                temp_sig2.insert(end(temp_sig2), begin(cA[i]), end(cA[i]));
            }
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cH[i]), end(cH[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cV[i]), end(cV[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cD[i]), end(cD[i]));
        }

        dwt_output.insert(dwt_output.begin(), temp_sig2.begin(), temp_sig2.end());
        breakPoint += temp_sig2.size();
        boundaries.push_back(breakPoint);
    }
    // Re-order boundaries so that we can identify the coefficients of the various transforms
    reverse(boundaries.begin(), boundaries.end());
    long dim = boundaries[0];
    for (auto &it : boundaries) {
        it = dim - it;
    }
    boundaries.push_back(dim);

    return 0;
}


void* idwt1_sym(QString name, vector<double> &X, vector<double> &app, vector<double> &detail)
{
    // Not Tested. Use dwt_sym and idwt_sym for any and all computations
    vector<double> dwtop;
    vector<double> flag;
    vector<double> idwt_output;
    vector<int> length;
    length[0] = app.size();
    length[1] = detail.size();
    dwtop = app;
    dwtop.insert(dwtop.end(),detail.begin(),detail.end());
    // Qt5 rewrite
    // dwtop.append(detail);
    flag.push_back(1);
    flag.push_back(0);
    idwt_sym(dwtop,flag,name,idwt_output,length);
    X = idwt_output;

    return 0;
}

void* idwt1_sym_m(QString name, vector<double> &idwt_output, vector<double> &app, vector<double> &detail)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    // Upsampling Factor
    int U = 2;
    int lf = lp2.size();

    // Operations in the Low Frequency branch of the Synthesis Filter Bank
    vector<double> X_lp;
    vector<double> cA_up;
    upsamp(app, U, cA_up );
    cA_up.pop_back();
    convfftm(cA_up, lp2, X_lp);

    // Operations in the High Frequency branch of the Synthesis Filter Bank
    vector<double> X_hp;
    vector<double> cD_up;
    upsamp(detail, U, cD_up);
    cD_up.pop_back();
    convfftm(cD_up, hp2, X_hp);

    vecsum(X_lp, X_hp, idwt_output);

    idwt_output.erase(idwt_output.begin(), idwt_output.begin()+lf-2);
    idwt_output.erase(idwt_output.end()-(lf-2), idwt_output.end());

    return 0;
}

void* symm_ext(vector<double> &sig, int a) {
    unsigned int len = sig.size();
    for (int i=0; i<a; ++i) {
        sig.insert(sig.begin(), sig[i*2]);
        sig.insert(sig.end(), sig[len-1]);
    }

    return 0;
}

void* idwt_sym(vector<double> &dwtop, vector<double> &flag, QString name,
               vector<double> &idwt_output, vector<int> &length)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    // Upsampling Factor
    int U = 2;
    int lf = lp2.size();

    int J = (int) flag[1];

    vector<double> app;
    vector<double> detail;
    unsigned int app_len = length[0];
    unsigned int det_len = length[1];

    vector<double>::iterator dwt;
    dwt = dwtop.begin();
    app.assign(dwt, dwtop.begin()+app_len);
    detail.assign(dwtop.begin()+app_len, dwtop.begin() + 2*app_len);

    // Qt5 rewrite; are the indices correct?
    //vector<double> app = dwtop.mid(0,app_len);
    //vector<double> detail = dwtop.mid(app_len,2*app_len);

    for (int i=0; i<J; ++i) {

        // Operations in the Low Frequency branch of the Synthesis Filter Bank
        vector<double> X_lp;
        vector<double> cA_up;
        upsamp(app, U, cA_up );
        cA_up.pop_back();
        convfft(cA_up, lp2, X_lp);

        // Operations in the High Frequency branch of the Synthesis Filter Bank
        vector<double> X_hp;
        vector<double> cD_up;
        upsamp(detail, U, cD_up);
        cD_up.pop_back();
        convfft(cD_up, hp2, X_hp);

        app_len += det_len;
        vecsum(X_lp, X_hp, idwt_output);

        idwt_output.erase(idwt_output.begin(), idwt_output.begin()+lf-2);
        idwt_output.erase(idwt_output.end()-(lf - 2), idwt_output.end());

        app.clear();
        detail.clear();
        if (i<J-1) {
            det_len = length[i+2];
            for (unsigned int l=0; l<det_len; ++l) {
                detail.push_back(dwtop[app_len+l]);
            }
        }
        app = idwt_output;

        for (int iter1=0; iter1 < (int) (app.size() - det_len); ++iter1) {
            app.pop_back();
        }
    }

    // Remove ZeroPadding
    int zerop =(int) flag[0];
    idwt_output.erase(idwt_output.end()-zerop, idwt_output.end());
    return 0;
}

void* dwt1_sym(const QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    // Downsampling factor
    int D = 2;
    int lf = lp1.size();
    symm_ext(signal, lf-1);

    // Low Pass Branch Computation
    vector<double> cA_undec;
    convfft(signal, lp1, cA_undec);
    cA_undec.erase(cA_undec.begin(), cA_undec.begin()+lf);
    cA_undec.erase(cA_undec.end()-lf+1, cA_undec.end());
    downsamp(cA_undec, D, cA);

    // High Pass Branch Computation
    vector<double> cD_undec;
    convfft(signal, hp1, cD_undec);
    cD_undec.erase(cD_undec.begin(), cD_undec.begin()+lf);
    cD_undec.erase(cD_undec.end()-lf+1, cD_undec.end());
    downsamp(cD_undec, D, cD);

    return 0;
}

void* dwt1_sym_m(const QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    // Downsampling factor
    int D = 2;
    int lf = lp1.size();
    symm_ext(signal, lf-1);

    // Low Pass Branch Computation
    vector<double> cA_undec;
    convfftm(signal, lp1, cA_undec);
    cA_undec.erase(cA_undec.begin(), cA_undec.begin()+lf);
    cA_undec.erase(cA_undec.end()-lf+1, cA_undec.end());
    downsamp(cA_undec, D, cA);

    // High Pass Branch Computation
    vector<double> cD_undec;
    convfftm(signal, hp1, cD_undec);
    cD_undec.erase(cD_undec.begin(), cD_undec.begin()+lf);
    cD_undec.erase(cD_undec.end()-lf+1, cD_undec.end());
    downsamp(cD_undec, D, cD);

    return 0;
}

void* dwt_sym(vector<double> &signal, int J,QString name, vector<double> &dwt_output,
              vector<double> &flag, vector<int> &length)
{
    unsigned int temp_len = signal.size();
    if ( (temp_len % 2) != 0) {
        signal.push_back(signal[temp_len - 1]);
        flag.push_back(1);
        temp_len++;
    } else {
        flag.push_back(0);
    }
    length.push_back(temp_len);
    flag.push_back(double(J));
    // flag[2] contains symmetric extension length


    vector<double> original_copy, appx_sig, det_sig;
    original_copy = signal;

    for (int iter = 0; iter < J; ++iter) {
        dwt1_sym(name,signal, appx_sig, det_sig);
        dwt_output.insert(dwt_output.begin(),det_sig.begin(),det_sig.end());
        // Qt5 rewrite
        //vector<double>::iterator itpos = dwt_output.begin();
        //for (auto &it: det_sig) {
        //    dwt_output.insert(itpos,it);
        //    ++itpos;
        // }
        int l_temp = det_sig.size();
        length.insert(length.begin(),l_temp);

        if (iter == J-1 ) {
            dwt_output.insert(dwt_output.begin(),appx_sig.begin(),appx_sig.end());
            // Qt5 rewrite
            //itpos = dwt_output.begin();
            //for (auto &it: det_sig) {
            //    dwt_output.insert(itpos,it);
            //    ++itpos;
            // }
            int l_temp = appx_sig.size();
            length.insert(length.begin(),l_temp);
        }

        signal.clear();
        signal = appx_sig;
        appx_sig.clear();
        det_sig.clear();
    }
    signal = original_copy;

    return 0;
}

/*
void* freq(vector<double> &sig, vector<double> &freq_resp)
{
    long K = sig.size();
    long N = pow(2.0, ceil(log10 (static_cast<double>(K))/log10(2.0)));
    vector<complex<double> > fft_oup;
    long sig_size = sig.size();
    for (long i=0; i<sig_size; ++i) {
        fft_oup.push_back(complex<double>(sig[i], 0));
    }
    fft(fft_oup, 1, N);

    for (long i=0; i<N; ++i) {
        freq_resp.push_back(abs(fft_oup[i]));
    }
    circshift(freq_resp, N/2);
    return 0;
}
*/

double convfft(const vector<double> &a, const vector<double> &b, vector<double> &c)
{
    fftw_complex *inp_data, *filt_data, *inp_fft, *filt_fft, *temp_data, *temp_ifft;
    fftw_plan plan_forward_inp, plan_forward_filt, plan_backward;

    long sz = a.size() + b.size() - 1;
    inp_data  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    filt_data = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    inp_fft   = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    filt_fft  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    temp_data = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    temp_ifft = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );

    plan_forward_inp  = fftw_plan_dft_1d( sz, inp_data, inp_fft, FFTW_FORWARD, FFTW_ESTIMATE );
    plan_forward_filt = fftw_plan_dft_1d( sz, filt_data, filt_fft, FFTW_FORWARD, FFTW_ESTIMATE );
    plan_backward     = fftw_plan_dft_1d( sz, temp_data, temp_ifft, FFTW_BACKWARD, FFTW_ESTIMATE );

    long a_size = a.size();
    long b_size = b.size();
    for (long i=0; i<sz; ++i) {
        if (i<a_size) inp_data[i][0] = a[i];
        else inp_data[i][0] = 0.0;

        inp_data[i][1] = 0.0;
        if (i<b_size) filt_data[i][0] = b[i];
        else filt_data[i][0] = 0.0;

        filt_data[i][1] = 0.0;
    }

    fftw_execute(plan_forward_inp);
    fftw_execute(plan_forward_filt);

    for (int i=0; i<sz; ++i) {
        temp_data[i][0] = inp_fft[i][0]*filt_fft[i][0] - inp_fft[i][1]*filt_fft[i][1];
        temp_data[i][1] = inp_fft[i][0]*filt_fft[i][1] + inp_fft[i][1]*filt_fft[i][0];
    }

    fftw_execute(plan_backward);

    c.reserve(sz);
    for (int i=0; i<sz; ++i) {
        c.push_back(temp_ifft[i][0] / (double) sz);
    }
    fftw_free(inp_data);
    fftw_free(filt_data);
    fftw_free(inp_fft);
    fftw_free(filt_fft);
    fftw_free(temp_data);
    fftw_free(temp_ifft);
    fftw_destroy_plan(plan_forward_inp);
    fftw_destroy_plan(plan_forward_filt);
    fftw_destroy_plan(plan_backward);

    return 0;
}

double convfftm(const vector<double> &a, const vector<double> &b, vector<double> &c)
{
    if (transient_size_of_fft == 0) fftw_init_threads();
    fftw_plan_with_nthreads(omp_get_max_threads());

    fftw_complex *inp_data, *filt_data, *inp_fft, *filt_fft, *temp_data, *temp_ifft;

    long sz = a.size() + b.size() - 1;
    inp_data  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    filt_data = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    inp_fft   = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    filt_fft  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    temp_data = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );
    temp_ifft = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * sz );

    if (sz != transient_size_of_fft) {
        if (transient_size_of_fft != 0) {
            fftw_destroy_plan(plan_forward_inp);
            fftw_destroy_plan(plan_forward_filt);
            fftw_destroy_plan(plan_backward);
        }
        plan_forward_inp  = fftw_plan_dft_1d( sz, inp_data, inp_fft, FFTW_FORWARD, FFTW_MEASURE );
        plan_forward_filt = fftw_plan_dft_1d( sz, filt_data, filt_fft, FFTW_FORWARD, FFTW_MEASURE );
        plan_backward     = fftw_plan_dft_1d( sz, temp_data, temp_ifft, FFTW_BACKWARD, FFTW_MEASURE );
        transient_size_of_fft = sz;
    }

    long a_size = a.size();
    long b_size = b.size();
    for (long i=0; i<sz; ++i) {
        if (i<a_size) inp_data[i][0] = a[i];
        else inp_data[i][0] = 0.0;

        inp_data[i][1] = 0.0;
        if (i<b_size) filt_data[i][0] = b[i];
        else filt_data[i][0] = 0.0;

        filt_data[i][1] = 0.0;
    }

    fftw_execute_dft( plan_forward_inp, inp_data, inp_fft);
    fftw_execute_dft( plan_forward_filt, filt_data, filt_fft);

    for (long i=0; i<sz; ++i) {
        temp_data[i][0] = inp_fft[i][0]*filt_fft[i][0] - inp_fft[i][1]*filt_fft[i][1];
        temp_data[i][1] = inp_fft[i][0]*filt_fft[i][1] + inp_fft[i][1]*filt_fft[i][0];
    }

    fftw_execute_dft( plan_backward, temp_data, temp_ifft);

    c.reserve(sz);
    for (long i=0; i<sz; ++i) {
        c.push_back(temp_ifft[i][0] / (double) sz);
    }
    fftw_free(inp_data);
    fftw_free(filt_data);
    fftw_free(inp_fft);
    fftw_free(filt_fft);
    fftw_free(temp_data);
    fftw_free(temp_ifft);

    return 0;
}

void* fft(vector<complex<double> > &data, int sign,unsigned  int N)
{
    double pi = - 3.14159265358979;
    if ( sign == 1 || sign == -1) {
        pi = sign*pi;
    } else {
        qDebug() << "Format fft(data, num), num = +1(fft) and num = -1 (Ifft)";
        exit(1);
    }
    unsigned int len = data.size();
    vector<complex<double> >::iterator it;
    it = data.end();
    if ( len != N) {
        unsigned int al = N - len;
        data.insert(it,al,complex<double>(0,0));
    }

    unsigned int K = (unsigned int) pow(2.0, ceil(log10(static_cast<double>(N))/log10(2.0)));
    vector<complex<double> >::iterator it1;
    it1 = data.end();
    if ( N < K) {
        unsigned int al = K - N;
        data.insert(it1,al,complex<double>(0,0));
        N = K;
    }

    bitreverse(data);

    //	 radix2(data);
    for (unsigned int iter = 1; iter < N; iter <<=1) {
        const unsigned int step = iter << 1;
        const double theta =  pi / double(iter);
        double wtemp = sin(theta*.5);
        //   Multipliers
        double wreal = -2*wtemp*wtemp;
        double wimag = sin(theta);
        //   Factors
        double wr = 1.0;
        double wi = 0.0;

        //   Iteration through two loops
        for (unsigned int m = 0; m < iter; m++) {
            //   Iteration within m
            for (unsigned int i = m; i<N; i += step) {
                //   jth position
                const unsigned int j = i + iter;
                double tempr = wr*real(data[j]) - wi*imag(data[j]);
                double tempi = wr*imag(data[j]) + wi*real(data[j]);
                complex<double> temp(tempr,tempi);
                data[j] = data[i] - temp;
                data[i] += temp;
            }
            //   Twiddle Factors updated
            wtemp = wr;
            wr += wr*wreal - wi*wimag;
            wi += wi*wreal + wtemp*wimag ;
        }
    }

    if ( sign == -1) {
        double scale = 1.0/double(N);
        for (unsigned int i=0; i<N; ++i) {
            data[i] *= scale;
        }
    }

    // Place holder
    return 0;
}


void* bitreverse(vector<complex<double> > &sig)
{
    unsigned int len = sig.size();
    unsigned int N = (unsigned int) pow(2.0, ceil(log10(static_cast<double>(len))/log10(2.0)));
    unsigned int rev = 0;
    //   Processing Input Data
    for (unsigned int iter = 0; iter < N; ++iter) {
        if (rev > iter) {
            // Replacing current values with reversed values
            complex<double> temp(real(sig[rev]), imag(sig[rev]));
            sig[rev] = sig[iter];
            sig[iter] = temp;
        }
        //   Using filter "filt" such that the value of reverse changes with each iteration
        unsigned int filt = N;
        while (rev & (filt >>= 1)) {
            rev &= ~filt;
        }
        rev |= filt;
    }
    return 0;
}


void* dwt(vector<double> &sig, int J, QString name, vector<double> &dwt_output,
          vector<double> &flag, vector<int> &length)
{
    int Max_Iter = ceil(log( double(sig.size()))/log (2.0)) - 2;
    if (Max_Iter < J) J = Max_Iter;

    vector<double> original_copy,orig, appx_sig, det_sig;
    original_copy = sig;

    // Zero Pad the Signal to nearest 2^ M value ,where M is an integer.
    unsigned int temp_len = sig.size();
    if ( (temp_len % 2) != 0) {
        sig.push_back(sig[temp_len-1]);
        flag.push_back(1);
        temp_len++;
    } else {
        flag.push_back(0);
    }
    length.push_back(temp_len);
    flag.push_back(double(J));

    orig = sig;

    for (int iter = 0; iter < J; ++iter) {
        dwt1(name, orig, appx_sig, det_sig);
        dwt_output.insert(dwt_output.begin(), det_sig.begin(), det_sig.end());
        // Qt5 rewrite
        //vector<double>::iterator itpos = dwt_output.begin();
        //for (auto &it: det_sig) {
        //    dwt_output.insert(itpos,it);
        //    ++itpos;
        //}

        length.insert(length.begin(),det_sig.size());

        if (iter == J-1 ) {
            dwt_output.insert(dwt_output.begin(), appx_sig.begin(), appx_sig.end());
            // Qt5 rewrite
            //itpos = dwt_output.begin();
            //for (auto &it: appx_sig) {
            //   dwt_output.insert(itpos,it);
            //   ++itpos;
            //}
            length.insert(length.begin(),appx_sig.size());
        }

        orig = appx_sig;
        appx_sig.clear();
        det_sig.clear();
    }

    sig = original_copy;
    return 0;
}


void circshift(vector<double> &sig_cir, int L)
{
    if ( abs(L) >(signed int) sig_cir.size()) {
        L = sign(L) * (abs(L) % sig_cir.size());
    }

    if ( L < 0 ) {
        L = (sig_cir.size() + L) % sig_cir.size();
    }
    for (int i=0; i<L; ++i) {
        sig_cir.push_back(sig_cir[0]);
        sig_cir.erase(sig_cir.begin());
    }
}

/*
double convol(vector<double> &a1, vector<double> &b1, vector<double> &c)
{
    unsigned int len_c = a1.size() + b1.size() - 1;
    vector<double> a = a1;
    vector<double> b = b1;
    double*  oup = NULL;

    oup = new double[len_c];
    vector<double>::iterator a_it;
    a_it = a.end();
    signed int al = len_c - a.size();
    a.insert(a_it, al, 0);

    vector<double>::iterator b_it;
    b_it = b.end();
    signed int bl = len_c - b.size();
    b.insert(b_it, bl, 0);

    for (unsigned int ini = 0; ini<len_c ; ++ini) {
        double ou1 = 0;
        oup[ini] = 0;
        for (unsigned int jni = 0; jni <= ini; ++jni) {
            ou1 = a[jni] * b[ini - jni];
            oup[ini] += ou1;
        }
        c.push_back(oup[ini]);
    }
    delete [] oup;
    oup = NULL;
    return 0;
}
*/

void downsamp(vector<double> &sig, int M, vector<double> &sig_d)
{
    int len = sig.size();
    long len_n = ceil( (double) len / (double) M);
    for (long i=0; i<len_n; ++i) {
        sig_d.push_back(sig[i*M]);
    }
}

void* dwt1(QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    int len_coeff = lp1.size();
    int len_sig = 2 * ceil((double) signal.size() / 2.0);

    // Periodic Extension
    per_ext(signal, len_coeff/2);

    // Downsampling factor
    int D = 2;

    // Low Pass Filtering Operations in the Analysis Filter Bank Section
    vector<double> cA_undec;
    // convolving signal with lp1, Low Pass Filter, and O/P is stored in cA_undec
    convfft(signal, lp1, cA_undec);
    cA_undec.erase(cA_undec.begin(), cA_undec.begin()+len_coeff-1);
    cA_undec.erase(cA_undec.end()-len_coeff+1, cA_undec.end());
    cA_undec.erase(cA_undec.begin()+len_sig, cA_undec.end());
    cA_undec.erase(cA_undec.begin());
    downsamp(cA_undec, D, cA);

    // High Pass Filtering Operations in the Analysis Filter Bank Section
    vector<double> cD_undec;
    // convolving signal with lp1, High Pass Filter, and O/P is stored in cD_undec
    convfft(signal, hp1, cD_undec);
    cD_undec.erase(cD_undec.begin(), cD_undec.begin()+len_coeff-1);
    cD_undec.erase(cD_undec.end()-len_coeff+1, cD_undec.end());
    cD_undec.erase(cD_undec.begin()+len_sig, cD_undec.end());
    cD_undec.erase(cD_undec.begin());
    downsamp(cD_undec, D, cD);

    return 0;
}

void* dwt1_m(QString name, vector<double> &signal, vector<double> &cA, vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    int len_coeff = lp1.size();
    int len_sig = 2 * ceil((double) signal.size() / 2.0);

    // Periodic Extension
    per_ext(signal, len_coeff/2);

    // Downsampling factor
    int D = 2;

    // Low Pass Filtering Operations in the Analysis Filter Bank Section
    vector<double> cA_undec;
    // convolving signal with Low Pass Filter, and O/P is stored in cA_undec
    convfftm(signal, lp1, cA_undec);
    cA_undec.erase(cA_undec.begin(), cA_undec.begin()+len_coeff-1);
    cA_undec.erase(cA_undec.end()-len_coeff+1, cA_undec.end());
    cA_undec.erase(cA_undec.begin()+len_sig, cA_undec.end());
    cA_undec.erase(cA_undec.begin());
    downsamp(cA_undec, D, cA);

    // High Pass Filtering Operations in the Analysis Filter Bank Section
    vector<double> cD_undec;
    // convolving signal with High Pass Filter, and O/P is stored in cD_undec
    convfftm(signal, hp1, cD_undec);
    cD_undec.erase(cD_undec.begin(), cD_undec.begin()+len_coeff-1);
    cD_undec.erase(cD_undec.end()-len_coeff+1, cD_undec.end());
    cD_undec.erase(cD_undec.begin()+len_sig, cD_undec.end());
    cD_undec.erase(cD_undec.begin());
    downsamp(cD_undec, D, cD);

    return 0;
}

void* dyadic_zpad_1d(vector<double> &signal)
{
    unsigned int N = signal.size();
    double M = log10 (static_cast<double> (N)) / log10(2.0);
    int D = ceil(M);
    double int_val = pow(2.0, double(D)) - pow(2.0, M);

    int z = (int) int_val;
    vector<double>::iterator a_it;
    a_it = signal.end();
    double val = signal[N-1];
    //  double val = 0;
    signal.insert(a_it,z,val);
    return 0;
}

void* idwt(vector<double> &dwtop, vector<double> &flag, QString name,
           vector<double> &idwt_output, vector<int> &length)
{

    int J = (int) flag[1];

    vector<double> app;
    vector<double> detail;
    unsigned int app_len = length[0];
    unsigned int det_len = length[1];

    vector<double>::iterator dwt;
    dwt = dwtop.begin();
    app.assign(dwt,dwtop.begin()+app_len);
    detail.assign(dwtop.begin()+app_len, dwtop.begin()+ 2* app_len);

    // Qt5 rewrite
    //vector<double> app = dwtop.mid(0,app_len);
    //vector<double> detail = dwtop.mid(app_len,2*app_len);

    for (int i=0; i<J; ++i) {
        idwt1(name,idwt_output, app,detail);
        app_len +=det_len;
        app.clear();
        detail.clear();
        if ( i<J-1 ) {
            det_len = length[i+2];
            for (unsigned int l=0; l<det_len; ++l) {
                detail.push_back(dwtop[app_len+l]);
            }
            app = idwt_output;

            if (app.size() >= detail.size()) {
                int t = app.size() - detail.size();
                int lent = (int) floor((double)t/2.0);
                app.erase(app.begin()+detail.size()+lent, app.end());
                app.erase(app.begin(), app.begin()+lent);
            }
        }
    }

    // Remove ZeroPadding
    int zerop =(int) flag[0];
    idwt_output.erase(idwt_output.end()- zerop, idwt_output.end());

    return 0;
}

void* idwt1_m(QString name, vector<double> &X, const vector<double> &cA, const vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    int len_coeff = lp1.size();
    unsigned int N = 2 * cD.size();
    int U = 2; // Upsampling Factor

    // Operations in the Low Frequency branch of the Synthesis Filter Bank
    vector<double> cA_up;
    vector<double> X_lp;
    upsamp(cA, U, cA_up);
    per_ext(cA_up, len_coeff/2);
    convfftm(cA_up, lp2, X_lp);

    // Operations in the High Frequency branch of the Synthesis Filter Bank
    vector<double> cD_up;
    vector<double> X_hp;
    upsamp(cD, U, cD_up);
    per_ext(cD_up, len_coeff/2);
    convfftm(cD_up, hp2, X_hp);

    // Remove periodic extension
    X_lp.erase(X_lp.begin()+N+len_coeff-1, X_lp.end());
    X_lp.erase(X_lp.begin(), X_lp.begin()+len_coeff-1);
    X_hp.erase(X_hp.begin()+N+len_coeff-1, X_hp.end());
    X_hp.erase(X_hp.begin(), X_hp.begin()+len_coeff-1);

    vecsum(X_lp, X_hp, X);

    return 0;
}

void* idwt1(QString name, vector<double> &X, vector<double> &cA, vector<double> &cD)
{
    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);

    int len_coeff = lp1.size();
    unsigned int N = 2 * cD.size();
    int U = 2; // Upsampling Factor

    // Operations in the Low Frequency branch of the Synthesis Filter Bank
    vector<double> cA_up;
    vector<double> X_lp;
    upsamp(cA, U, cA_up);
    per_ext(cA_up, len_coeff/2);
    convfft(cA_up, lp2, X_lp);

    // Operations in the High Frequency branch of the Synthesis Filter Bank
    vector<double> cD_up;
    vector<double> X_hp;
    upsamp(cD, U, cD_up);
    per_ext(cD_up, len_coeff/2);
    convfft(cD_up, hp2, X_hp);

    // Remove periodic extension
    X_lp.erase(X_lp.begin()+N+len_coeff-1, X_lp.end());
    X_lp.erase(X_lp.begin(), X_lp.begin()+len_coeff-1);
    X_hp.erase(X_hp.begin()+N+len_coeff-1, X_hp.end());
    X_hp.erase(X_hp.begin(), X_hp.begin()+len_coeff-1);

    vecsum(X_lp, X_hp, X);

    return 0;
}

int sign(int X) {
    if (X >= 0)
        return 1;
    else
        return -1;
}

void upsamp(const vector<double> &sig, int M, vector<double> &sig_u)
{
    long len = sig.size() * M;
    sig_u.clear();
    sig_u.reserve(len);

    for (long i=0; i<len; ++i) {
        if ( i % M == 0) sig_u.push_back(sig[i/M]);
        else sig_u.push_back(0);
    }
}

int vecsum(vector<double> &a, vector<double> &b, vector<double> &c)
{
    c.resize(a.size());
    transform (a.begin(), a.end(), b.begin(), b.begin(), plus<double>());
    c = b;
    return 0;
}

void* getcoeff2d(vector<vector<double> > &dwtoutput, vector<vector<double> > &cH,
                 vector<vector<double> > &cV, vector<vector<double> > &cD, vector<double> &flag, int &N)
{
    if (N > flag[2]) {
        qDebug() << "Signal is decimated only up to " << flag[2] << " levels.";
        exit(1);
    }
    int rows = dwtoutput.size();
    int cols = dwtoutput[0].size();
    // Getting Horizontal Coefficients
    int r = ceil((double) rows /pow(2.0,N)) ;
    int c = ceil((double) cols /pow(2.0,N)) ;

    for (int i=0; i<ceil ((double) rows /pow(2.0,N)); ++i) {
        for (int j=0; j<ceil ((double) cols /pow(2.0,N)); ++j) {
            cH[i][j] = dwtoutput[i][c+j];
        }
    }

    for (int i=0; i<ceil ((double) rows /pow(2.0,N)); ++i) {
        for (int j=0; j<ceil ((double) cols /pow(2.0,N)); ++j) {
            cV[i][j] = dwtoutput[i+r][j];
        }
    }

    for (int i=0; i<ceil ((double) rows /pow(2.0,N)); ++i) {
        for (int j=0; j<ceil ((double) cols /pow(2.0,N)); ++j) {
            cD[i][j] = dwtoutput[i+r][c+j];
        }
    }

    return 0;
}

void* zero_remove(vector<vector<double> > &input, vector<vector<double> > &output)
{
    int zero_rows = output.size() - input.size();
    int zero_cols = output[0].size() - input[0].size();

    vector<vector<double> >::iterator row = output.end()-zero_rows;

    unsigned int ousize = output.size();
    for (unsigned int i = input.size(); i<ousize; ++i) {
        output.erase(row);
        row++;
    }

    for (unsigned int i=0; i<ousize; ++i) {
        vector<double> ::iterator col = output[i].end()-zero_cols;

        output[i].erase(col, output[i].end());
    }
    return 0;
}

void* dwt_output_dim(vector<vector<double> >&signal, int &r, int &c)
{
    int rows =signal.size();
    int cols = signal[0].size();

    double Mr = log10 (static_cast<double> (rows)) / log10(2.0);
    int Dr = ceil(Mr);
    double int_val_row = pow(2.0, double(Dr));
    int r1 = (int) int_val_row;

    double Mc = log10 (static_cast<double> (cols)) / log10(2.0);
    int Dc = ceil(Mc);
    double int_val_cols = pow(2.0, double(Dc));
    int c1 = (int) int_val_cols;
    r=max(r1,c1);
    c=max(r1,c1);

    return 0;
}

void* dyadic_zpad_2d(vector<vector<double> > &signal, vector<vector<double> > &mod)
{
    int rows = signal.size();
    int cols = signal[0].size();

    for (int i=0; i<rows; ++i) {
        for (int j = 0; j<cols; ++j) {
            mod[i][j] = signal[i][j];
        }
    }
    // Zeropadding the columns

    double Mr = log10 (static_cast<double> (rows)) / log10(2.0);
    int Dr = ceil(Mr);
    double int_val_row = pow(2.0, double(Dr)) - pow(2.0, Mr);

    int zeros_row = (int) int_val_row;

    double Mc = log10 (static_cast<double> (cols)) / log10(2.0);
    int Dc = ceil(Mc);
    double int_val_cols = pow(2.0, double(Dc)) - pow(2.0, Mc);

    int zeros_cols = (int) int_val_cols;

    for (int i=0; i<rows + zeros_row; ++i) {
        for (int j = cols; j<cols+zeros_cols; ++j) {
            mod[i][j] = 0;
        }
    }

    for (int i=rows; i<rows + zeros_row; ++i) {
        for (int j = 0; j<cols+zeros_cols; ++j) {
            mod[i][j] = 0;
        }
    }

    return 0;
}

void* idwt_2d(vector<double> &dwtop, vector<double> &flag, QString name,
              vector<vector<double> > &idwt_output, vector<int> &length)
{
    int J = (int) flag[0];
    int rows = length[0];
    int cols = length[1];
    int sum_coef = 0;
    long oup_size = 0;
    vector<vector<double> > cLL(rows, vector<double>(cols));

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int len_coeff = lp1.size();

    for (int iter=0; iter<J; ++iter) {
        long rows_n = length[2*iter];
        long cols_n = length[2*iter+1];
        vector<vector<double> > cLH(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHL(rows_n, vector<double>(cols_n));
        vector<vector<double> > cHH(rows_n, vector<double>(cols_n));

        for (long i=0; i<rows_n; ++i ) {
            for (long j=0; j<cols_n; ++j) {
                if (iter == 0) {
                    cLL[i][j] = dwtop[sum_coef + i*cols_n + j];
                    cLH[i][j] = dwtop[sum_coef + i*cols_n + j + 1*rows_n*cols_n];
                    cHL[i][j] = dwtop[sum_coef + i*cols_n + j + 2*rows_n*cols_n];
                    cHH[i][j] = dwtop[sum_coef + i*cols_n + j + 3*rows_n*cols_n];
                }
                else {
                    cLH[i][j] = dwtop[sum_coef + i*cols_n + j];
                    cHL[i][j] = dwtop[sum_coef + i*cols_n + j + 1*rows_n*cols_n];
                    cHH[i][j] = dwtop[sum_coef + i*cols_n + j + 2*rows_n*cols_n];
                }
            }
        }

        long len_x = cLH.size();
        long len_y = cLH[0].size();

        // Row Upsampling and Column Filtering at the first LP Stage
        vector<vector<double> > cL(2*len_x, vector<double>(len_y));
        vector<vector<double> > cH(2*len_x, vector<double>(len_y));

        for (long j=0; j<len_y; ++j) {
            vector<double> sigLL;
            vector<double> sigLH; // cannot define vector with this size directly. Mem error in idwt_m() line: X_lp.erase(X_lp.begin(), X_lp.begin()+len_coeff-1);
            vector<double> sigHL;
            vector<double> sigHH; // cannot define vector with this size directly. Mem error in idwt_m() line: X_lp.erase(X_lp.begin(), X_lp.begin()+len_coeff-1);
            // reserve() works fine, though.
            sigLL.reserve(len_x);
            sigLH.reserve(len_x);
            sigHL.reserve(len_x);
            sigHH.reserve(len_x);
            for (long i=0; i<len_x; ++i) {
                sigLL.push_back(cLL[i][j]);
                sigLH.push_back(cLH[i][j]);
                sigHL.push_back(cHL[i][j]);
                sigHH.push_back(cHH[i][j]);
            }
            vector<double> oupL;
            vector<double> oupH;
            oupL.reserve(len_x*2+len_coeff);
            oupH.reserve(len_x*2+len_coeff);
            idwt1_m(name, oupL, sigLL, sigLH);
            idwt1_m(name, oupH, sigHL, sigHH);
            oup_size = oupL.size();
            for (long i=0; i<oup_size; ++i) {
                cL[i][j] = oupL[i];
                cH[i][j] = oupH[i];
            }
        }

        vector<vector<double> > signal(2*len_x, vector<double>(2*len_y));
        for (long i=0; i<2*len_x; ++i) {
            vector<double> oup;
            oup.reserve(2*len_x+len_coeff);
            idwt1_m(name, oup, cL[i], cH[i]);
            oup_size = oup.size();
            signal[i] = oup;
        }

        idwt_output = signal;

        if (iter == 0) sum_coef += 4*rows_n*cols_n;
        else sum_coef += 3*rows_n*cols_n;

        cLL = signal;
    }

    return 0;
}

void* dwt_2d(vector<vector<double> > &origsig, int J, QString name, vector<double> &dwt_output,
             vector<double> &flag, vector<int> &length, vector<long> &boundaries)
{
    vector<vector<double> > sig = origsig;
    vector<vector<double> > original_copy = origsig;
    int rows_n = sig.size();
    int cols_n = sig[0].size();

    int Max_Iter = min(ceil(log( double(sig.size()))/log (2.0)), ceil(log( double(sig[0].size()))/log (2.0)));
    if ( Max_Iter < J) {
        qDebug() << J << " iterations are not possible with signals of this dimension ";
        exit(1);
    }

    flag.push_back(double(J));
    flag.push_back(0);

    length.insert(length.begin(), cols_n);
    length.insert(length.begin(), rows_n);

    long breakPoint = 0;

    for (int iter=0; iter<J; ++iter) {
        rows_n = ceil((double) rows_n / 2.0);
        cols_n = ceil((double) cols_n / 2.0);
        length.insert(length.begin(), cols_n);
        length.insert(length.begin(), rows_n);

        vector<vector<double>> cA(rows_n, vector<double>(cols_n));
        vector<vector<double>> cH(rows_n, vector<double>(cols_n));
        vector<vector<double>> cV(rows_n, vector<double>(cols_n));
        vector<vector<double>> cD(rows_n, vector<double>(cols_n));
        dwt2(name, original_copy, cA, cH, cV, cD);

        vector<double> temp_sig2;
        temp_sig2.reserve(4*rows_n*cols_n);

        original_copy = cA;
        if (iter == J-1) {
            for (int i=0; i<rows_n; ++i) {
                temp_sig2.insert(end(temp_sig2), begin(cA[i]), end(cA[i]));
            }
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cH[i]), end(cH[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cV[i]), end(cV[i]));
        }

        for (int i=0; i<rows_n; ++i) {
            temp_sig2.insert(end(temp_sig2), begin(cD[i]), end(cD[i]));
        }

        // TODO: insert is inefficient!
        dwt_output.insert(dwt_output.begin(), temp_sig2.begin(), temp_sig2.end());
        breakPoint += temp_sig2.size();
        boundaries.push_back(breakPoint);
        // Qt5 rewrite
        //vector<double>::iterator itpos = dwt_output.begin();
        //for (auto &it: temp_sig2) {
        //    dwt_output.insert(itpos,it);
        //    ++itpos;
        //}
    }

    // Re-order boundaries so that we can identify the coefficients of the various transforms
    reverse(boundaries.begin(), boundaries.end());
    long dim = boundaries[0];
    for (auto &it : boundaries) {
        it = dim - it;
    }
    boundaries.push_back(dim);

    return 0;
}

void* branch_up(QString name, vector<double> &cA, vector<double> &cD, vector<double> &X)
{
    // Upsampling Factor
    int U = 2;

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int len_coeff = lp1.size();

    // Operations in the Low Frequency branch of the Synthesis Filter Bank
    vector<double> cA_up;
    vector<double> X_lp;
    per_ext(cA, len_coeff/2);
    upsamp(cA, U, cA_up);
    convfftm(cA_up, lp2, X_lp);

    // Operations in the High Frequency branch of the Synthesis Filter Bank
    vector<double> cD_up;
    vector<double> X_hp;
    per_ext(cD, len_coeff/2);
    upsamp(cD, U, cD_up);
    convfftm(cD_up, hp2, X_hp);

    vecsum(X_lp, X_hp, X);

    // Remove periodic extension
    X.erase(X.begin(), X.begin()+len_coeff+len_coeff/2-1);
    X.erase(X.end()-len_coeff-len_coeff/2, X.end());

    return 0;
}

void* branch_dn(QString name, vector<double> &signal, vector<double> &sigop, QString mode)
{
    // Downsampling factor
    int D = 2;

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int len_coeff = lp1.size();

    int temp_len = signal.size();
    if ( (temp_len % 2) != 0) signal.push_back(signal[temp_len-1]);

    per_ext(signal, len_coeff / 2);

    // Low-pass filtering: cX_undec corresponds to cA_undec
    // High-pass filtering: cX_undec corresponds to cD_undec
    vector<double> cX_undec;
    if (mode == "lowpass")  convfftm(signal, lp1, cX_undec);
    if (mode == "highpass") convfftm(signal, hp1, cX_undec);

    downsamp(cX_undec, D, sigop);

    sigop.erase(sigop.begin(), sigop.begin()+len_coeff/2);
    sigop.erase(sigop.end()-len_coeff/2, sigop.end());
    return 0;
}

void* idwt2(QString name, vector<vector<double> > &signal, vector<vector<double> > &cLL,
            vector<vector<double> > &cLH, vector<vector<double> > &cHL, vector<vector<double> > &cHH)
{
    // Synthesis
    long rows = cLL.size();
    long cols = cLL[0].size();
    long rows_n = 2 * rows;

    // Row Upsampling and Column Filtering at the first LP Stage
    vector<vector<double> > cL(rows_n, vector<double>(cols));
    vector<vector<double> > cH(rows_n, vector<double>(cols));
    idwt2_subroutine_A(name, cLL, cLH, cL, cols, rows);
    idwt2_subroutine_A(name, cHL, cHH, cH, cols, rows);
    idwt2_subroutine_B(name, cL, cH, signal, rows_n, cols);

    return 0;
}

void idwt2_subroutine_A(QString name, const vector<vector<double> > sigInL, const vector<vector<double> > sigInH,
                        vector<vector<double> > sigOut, const long index1, const long index2)
{
    for (long j=0; j<index1; ++j) {
        vector<double> sigL(index2);
        vector<double> sigH(index2);
        for (long i=0; i<index2; ++i) {
            sigL.push_back(sigInL[i][j]);
            sigH.push_back(sigInH[i][j]);
        }
        vector<double> oup;
        branch_up(name, sigL, sigH, oup);
        long oup_size = oup.size();
        for (long i=0; i<oup_size; ++i) {
            sigOut[i][j] = oup[i];
        }
    }
}

void idwt2_subroutine_B(QString name, vector<vector<double> > sigInL, vector<vector<double> > sigInH,
                        vector<vector<double> > sigOut, const long index1, const long index2)
{
    for (long i=0; i<index1; ++i) {
        vector<double> oup;
        branch_up(name, sigInL[i], sigInH[i], oup);
        sigOut[i] = oup;
    }
}

void* dwt2(QString name, vector<vector<double> > &signal, vector<vector<double> > &cLL,
           vector<vector<double> > &cLH, vector<vector<double> > &cHL, vector<vector<double> > &cHH)
{
    int rows = signal.size();
    int cols = signal[0].size();
    int cols_lp1 = cLL[0].size();
    int cols_hp1 = cLL[0].size();
    vector<vector<double> > lp_dn1(rows, vector<double>(cols_lp1));
    vector<vector<double> > hp_dn1(rows, vector<double>(cols_hp1));

    vector<double> lp1, hp1, lp2, hp2;
    filtcoef(name, lp1, hp1, lp2, hp2);
    int lf = lp1.size();

    // Implementing row filtering and column downsampling in each branch
    dwt2_subroutine_A(name, signal, lp_dn1, hp_dn1, rows, cols, lf, "mode_norm");

    // Column filtering and row downsampling in Low/High Pass branches.
    dwt2_subroutine_B(name, lp_dn1, cLL, cLH, cols_lp1, rows, lf, "mode_norm");
    dwt2_subroutine_B(name, hp_dn1, cHL, cHH, cols_hp1, rows, lf, "mode_norm");

    return 0;
}

void* downsamp2(vector<vector<double> > & vec1, vector<vector<double> > & vec2, int rows_dn, int cols_dn)
{
    int rows = vec1.size();
    int cols = vec1[0].size();
    int rows_n = ceil( (double) rows / (double) rows_dn);
    int cols_n = ceil( (double) cols / (double) cols_dn);
    for (int i=0; i<rows_n; ++i) {
        for (int j=0; j<cols_n; ++j) {
            vec2[i][j] = vec1[i*rows_dn][j*cols_dn];
        }
    }
    return 0;
}

void* upsamp2(vector<vector<double> > & vec1, vector<vector<double> > & vec2, int rows_up, int cols_up)
{
    int rows = vec1.size();
    int cols = vec1[0].size();
    int rows_n = rows *  rows_up;
    int cols_n =  cols *  cols_up;
    for (int i=0; i<rows_n; ++i) {
        for (int j=0; j<cols_n; ++j) {
            if ( i % rows_up == 0 && j % cols_up == 0) {
                vec2[i][j] = vec1[(int) (i/rows_up)][(int) (j/cols_up)];
            }
            else {
                vec2[i][j] = 0;
            }
        }
    }
    return 0;
}
