#include "dntheam.h"

DNTheam::DNTheam()
{
}

void DNTheam::SelectWater()
{

}

double DNTheam::Batinometr(int TypeBottom,float R485,float R560,float R660,float Ra425,float Ra545,float Ra605)
{
 double R1b=0.42,R2b=0.64,R3b=0.58;
 float Ra[3];
 Ra[0]=Ra425;
 Ra[1]=Ra545;
 Ra[2]=Ra605;

 bool FirstIter=TRUE;
 double aRes,bRes,hRes;

 BatimetrResult Res;


 TypeBottom=1;

 if(TypeBottom==0) //Макрофиты
 {
  R1b=0.02;
  R2b=0.06;
  R3b=0.065;
 }
 if(TypeBottom==1)  //Песок
 {
  R1b=0.03;
  R2b=0.092;
  R3b=0.11;
 }
 if(TypeBottom==2) //Водоросли
 {
  R1b=0.06;
  R2b=0.09;
  R3b=0.1;
 }

 double Schet=0,hMin,FMin;
 FirstIter=TRUE;
 bRes=0.05;
 aRes=0.5;
 double dR1,dR2;

// for (hRes=0.1; hRes<=15;hRes+=0.1)
// {
//  Res=this->CalcFunkBatimetr(aRes,bRes,hRes,R1a,R2a,R3a,R1b,R2b,R3b);
//  if(hRes==0.1)
//  {
//   ResProv1=this->CalcFunkBatimetrObr(Res.a,Res.b,0.1,R1b,R2b,R3b);
//   dR1=ResProv1.a*ResProv1.a+ResProv1.b*ResProv1.b+ResProv1.h*ResProv1.h;
//  }
//  if(hRes>0.1)
//  {
//   ResProv1=ResProv2;
//   dR1=dR2;
//   ResProv2=this->CalcFunkBatimetrObr(Res.a,Res.b,Res.h,R1b,R2b,R3b);
//   dR2=ResProv2.a*ResProv2.a+ResProv2.b*ResProv2.b+ResProv2.h*ResProv2.h;
//  }
//  if((hRes>0.1&&fabs((dR2-dR1)/(dR2+dR1))>=0.007)||hRes==0.1)
//  {
   Res.h=this->CalcFunkBatimetrNew(TypeBottom,R485,R560,R660,Ra);
//   QMessageBox msg;
//   msg.setText(//QString().setNum(Res.a,'d',4)+"\n"+
//               //QString().setNum(Res.b,'d',4)+"\n"+
//               //QString().setNum(Res.h,'d',4)+"\n"+
//               QString().setNum(Res.h,'d',4));
//   msg.exec();
//  }
// }//for (hRes=0.1; hRes<=10;hRes+=/*0.1*//*0.5)

 return Res.h;
}

QList<BigthResult> DNTheam::CalcFunkBatimetrObr(int TypeBottom)
{
 double f1,f2,f3;
 double d1,d2,d3,k1,k2,k3;
 double lamda1=425,lamda2=545,lamda3=605;
 double awLamda1=0.0045,awLamda2=0.056,awLamda3=0.240;  //показатель поглощения чистой морской водой
 double bwLamda1=0.004,bwLamda2=0.00135,bwLamda3=0.00093;  //показатель рассеивания чистой морской водой
 double ALamda1=0.0362,ALamda2=0.009,ALamda3=0.0056; //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double BLamda1=0.313,BLamda2=0.073,BLamda3=0.093;  //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double s500=0.017,s501=0.011; //коэффициенты для расчета показателя поглощения РОВ
 double Cph=0.1; //концентрация хлорофилла мг/м3
 double con1[8],con2[8],con3[8];

 double R1a,R2a,R3a;
 double R1b,R2b,R3b;

 BigthResult ReturnRes;
 QList <BigthResult> Ret;

 TypeBottom=1;

 if(TypeBottom==0) //Макрофиты
 {
  R1b=0.02;
  R2b=0.06;
  R3b=0.065;
 }
 if(TypeBottom==1)  //Песок
 {
  R1b=0.03;
  R2b=0.092;
  R3b=0.11;
 }
 if(TypeBottom==2) //Водоросли
 {
  R1b=0.06;
  R2b=0.09;
  R3b=0.1;
 }


 con1[0]=bwLamda1;
 con1[1]=pow((550/lamda1),(-1));
 con1[2]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[3]=exp(-s500*(lamda1-443));

 con1[4]=exp(-s500*(lamda1-443));
 con1[5]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[6]=pow((550/lamda1),(-1));
 con1[7]=bwLamda1;

 con2[0]=bwLamda2;
 con2[1]=pow((550/lamda2),-1);
 con2[2]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[3]=exp(-s501*(lamda2-443));

 con2[4]=exp(-s501*(lamda2-443));
 con2[5]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[6]=pow((550/lamda2),(-1));
 con2[7]=bwLamda2;

 con3[0]=bwLamda3;
 con3[1]=pow((550/lamda3),(-1));
 con3[2]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[3]=exp(-s501*(lamda3-443));

 con3[4]=exp(-s501*(lamda3-443));
 con3[5]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[6]=pow((550/lamda3),(-1));
 con3[7]=bwLamda3;

 for(double h=0.1; h<=15;h+=0.5)
 {
  for(double b=0.004; b<=0.1;b+=0.05)
  {
   for(double a=0.1; a<=1.12;a+=0.5)
   {
    d1=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
    d2=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
    d3=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);

    k1=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
    k2=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
    k3=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

    R1a=0.2895*d1*(1-k1)+R1b*k1;
    R2a=0.2895*d2*(1-k2)+R2b*k2;
    R3a=0.2895*d3*(1-k3)+R3b*k3;

    ReturnRes.a=a;
    ReturnRes.b=b;
    ReturnRes.h=h;
    ReturnRes.R1a=R1a;
    ReturnRes.R2a=R2a;
    ReturnRes.R3a=R3a;
    Ret<<ReturnRes;
   }//for(double a=0.1; a<=1.12;a+=0.5)
  }//for(double b=0.004; b<=0.1;b+=0.05)
 }//for(double h=0.1; h<=15;h+=0.5)
 return Ret;
}


BatimetrResult DNTheam::CalcFunkBatimetrObr(double a,double b,double h,double R1b,double R2b,double R3b)
{
 double f1,f2,f3;
 double d1,d2,d3,k1,k2,k3;
 double lamda1=425,lamda2=545,lamda3=605;
 double awLamda1=0.0045,awLamda2=0.056,awLamda3=0.240;  //показатель поглощения чистой морской водой
 double bwLamda1=0.004,bwLamda2=0.00135,bwLamda3=0.00093;  //показатель рассеивания чистой морской водой
 double ALamda1=0.0362,ALamda2=0.009,ALamda3=0.0056; //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double BLamda1=0.313,BLamda2=0.073,BLamda3=0.093;  //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double s500=0.017,s501=0.011; //коэффициенты для расчета показателя поглощения РОВ
 double Cph=0.1; //концентрация хлорофилла мг/м3
 double con1[8],con2[8],con3[8];

 double R1a,R2a,R3a;


 BatimetrResult ReturnRes;


 con1[0]=bwLamda1;
 con1[1]=pow((550/lamda1),(-1));
 con1[2]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[3]=exp(-s500*(lamda1-443));

 con1[4]=exp(-s500*(lamda1-443));
 con1[5]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[6]=pow((550/lamda1),(-1));
 con1[7]=bwLamda1;

 con2[0]=bwLamda2;
 con2[1]=pow((550/lamda2),-1);
 con2[2]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[3]=exp(-s501*(lamda2-443));

 con2[4]=exp(-s501*(lamda2-443));
 con2[5]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[6]=pow((550/lamda2),(-1));
 con2[7]=bwLamda2;

 con3[0]=bwLamda3;
 con3[1]=pow((550/lamda3),(-1));
 con3[2]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[3]=exp(-s501*(lamda3-443));

 con3[4]=exp(-s501*(lamda3-443));
 con3[5]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[6]=pow((550/lamda3),(-1));
 con3[7]=bwLamda3;


    d1=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
    d2=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
    d3=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);

    k1=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
    k2=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
    k3=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

    R1a=0.2895*d1*(1-k1)+R1b*k1;
    R2a=0.2895*d2*(1-k2)+R2b*k2;
    R3a=0.2895*d3*(1-k3)+R3b*k3;

    ReturnRes.a=R1a;
    ReturnRes.b=R2a;
    ReturnRes.h=R3a;


 return ReturnRes;
}

BatimetrResult DNTheam::CalcFunkBatimetr(double a,double b,double h,float R1a,float R2a,float R3a, double R1b, double R2b, double R3b)
{
 double f1,f2,f3,FMain;
 double d1,d2,d3,k1,k2,k3;
 double d_da[3],d_db[3],d_ka[3],d_kb[3],d_kh[3],d_fa[3],d_fb[3],d_fh[3];
 double ap,ao,bp,bo,hp,ho,dh=0.00001;
 double d_FMainA,d_FMainB,d_FMainH;
 double lamda1=425,lamda2=545,lamda3=605;
 double awLamda1=0.0045,awLamda2=0.056,awLamda3=0.240;  //показатель поглощения чистой морской водой
 double bwLamda1=0.004,bwLamda2=0.00135,bwLamda3=0.00093;  //показатель рассеивания чистой морской водой
 double ALamda1=0.0362,ALamda2=0.009,ALamda3=0.0056; //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double BLamda1=0.313,BLamda2=0.073,BLamda3=0.093;  //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double s500=0.017,s501=0.011; //коэффициенты для расчета показателя поглощения РОВ
 double Cph=0.1; //концентрация хлорофилла мг/м3
 double con1[8],con2[8],con3[8];

 double delta[3];
 BatimetrResult ReturnRes;

 ap=a+dh;
 ao=a-dh;
 bp=b+dh;
 bo=b-dh;
 hp=h+dh;
 ho=h-dh;

 con1[0]=bwLamda1;
 con1[1]=pow((550/lamda1),(-1));
 con1[2]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[3]=exp(-s500*(lamda1-443));

 con1[4]=exp(-s500*(lamda1-443));
 con1[5]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[6]=pow((550/lamda1),(-1));
 con1[7]=bwLamda1;

 con2[0]=bwLamda2;
 con2[1]=pow((550/lamda2),-1);
 con2[2]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[3]=exp(-s501*(lamda2-443));

 con2[4]=exp(-s501*(lamda2-443));
 con2[5]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[6]=pow((550/lamda2),(-1));
 con2[7]=bwLamda2;

 con3[0]=bwLamda3;
 con3[1]=pow((550/lamda3),(-1));
 con3[2]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[3]=exp(-s501*(lamda3-443));

 con3[4]=exp(-s501*(lamda3-443));
 con3[5]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[6]=pow((550/lamda3),(-1));
 con3[7]=bwLamda3;

 double I[9];
 int n=3;

 for(int jy=0;jy<n;jy++)
 {
  for(int jx=0;jx<n;jx++)
  {
   I[jx+jy*n]=0;
   if(jx==jy)
    I[jx+jy*n]=1;
  }
 }

 double dFMain;

 do
 {
 d1=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
 d2=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
 d3=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);

 double dap[3],dbp[3],dao[3],dbo[3],dhp[3],dho[3];
 /*Плюс*/
 dap[0]=(con1[0]+b*con1[1])/(con1[2]+ap*con1[3]);
 dap[1]=(con2[0]+b*con2[1])/(con2[2]+ap*con2[3]);
 dap[2]=(con3[0]+b*con3[1])/(con3[2]+ap*con3[3]);

 dbp[0]=(con1[0]+bp*con1[1])/(con1[2]+a*con1[3]);
 dbp[1]=(con2[0]+bp*con2[1])/(con2[2]+a*con2[3]);
 dbp[2]=(con3[0]+bp*con3[1])/(con3[2]+a*con3[3]);

 dhp[0]=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
 dhp[1]=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
 dhp[2]=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);

 /*Минус*/
 dao[0]=(con1[0]+b*con1[1])/(con1[2]+ao*con1[3]);
 dao[1]=(con2[0]+b*con2[1])/(con2[2]+ao*con2[3]);
 dao[2]=(con3[0]+b*con3[1])/(con3[2]+ao*con3[3]);

 dbo[0]=(con1[0]+bo*con1[1])/(con1[2]+a*con1[3]);
 dbo[1]=(con2[0]+bo*con2[1])/(con2[2]+a*con2[3]);
 dbo[2]=(con3[0]+bo*con3[1])/(con3[2]+a*con3[3]);

 dho[0]=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
 dho[1]=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
 dho[2]=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);


 k1=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 k2=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 k3=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));



 double kap[3],kbp[3],khp[3],kao[3],kbo[3],kho[3];
 kap[0]=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+ap*con1[4]));
 kap[1]=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+ap*con2[4]));
 kap[2]=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+ap*con3[4]));

 kao[0]=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+ao*con1[4]));
 kao[1]=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+ao*con2[4]));
 kao[2]=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+ao*con3[4]));

 kbp[0]=exp(-2*h*(con1[7]+bp*con1[6]+con1[5]+a*con1[4]));
 kbp[1]=exp(-2*h*(con2[7]+bp*con2[6]+con2[5]+a*con2[4]));
 kbp[2]=exp(-2*h*(con3[7]+bp*con3[6]+con3[5]+a*con3[4]));

 kbo[0]=exp(-2*h*(con1[7]+bo*con1[6]+con1[5]+a*con1[4]));
 kbo[1]=exp(-2*h*(con2[7]+bo*con2[6]+con2[5]+a*con2[4]));
 kbo[2]=exp(-2*h*(con3[7]+bo*con3[6]+con3[5]+a*con3[4]));

 khp[0]=exp(-2*hp*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 khp[1]=exp(-2*hp*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 khp[2]=exp(-2*hp*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

 kho[0]=exp(-2*ho*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 kho[1]=exp(-2*ho*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 kho[2]=exp(-2*ho*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

 /*Производные*/
 d_da[0]=(con1[0]+b*con1[1])*(-1)*con1[3]*(1/((con1[2]+a*con1[3])*(con1[2]+a*con1[3])));
 d_da[1]=(con2[0]+b*con2[1])*(-1)*con2[3]*(1/((con2[2]+a*con2[3])*(con2[2]+a*con2[3])));
 d_da[2]=(con3[0]+b*con3[1])*(-1)*con3[3]*(1/((con3[2]+a*con3[3])*(con3[2]+a*con3[3])));

 double d_daap[3],d_dabp[3],d_daao[3],d_dabo[3],d_dahp[3],d_daho[3];
 /*Плюс*/
 d_daap[0]=(con1[0]+b*con1[1])*(-1)*con1[3]*(1/((con1[2]+ap*con1[3])*(con1[2]+ap*con1[3])));
 d_daap[1]=(con2[0]+b*con2[1])*(-1)*con2[3]*(1/((con2[2]+ap*con2[3])*(con2[2]+ap*con2[3])));
 d_daap[2]=(con3[0]+b*con3[1])*(-1)*con3[3]*(1/((con3[2]+ap*con3[3])*(con3[2]+ap*con3[3])));

 d_dabp[0]=(con1[0]+bp*con1[1])*(-1)*con1[3]*(1/((con1[2]+a*con1[3])*(con1[2]+a*con1[3])));
 d_dabp[1]=(con2[0]+bp*con2[1])*(-1)*con2[3]*(1/((con2[2]+a*con2[3])*(con2[2]+a*con2[3])));
 d_dabp[2]=(con3[0]+bp*con3[1])*(-1)*con3[3]*(1/((con3[2]+a*con3[3])*(con3[2]+a*con3[3])));

 d_dahp[0]=(con1[0]+b*con1[1])*(-1)*con1[3]*(1/((con1[2]+a*con1[3])*(con1[2]+a*con1[3])));
 d_dahp[1]=(con2[0]+b*con2[1])*(-1)*con2[3]*(1/((con2[2]+a*con2[3])*(con2[2]+a*con2[3])));
 d_dahp[2]=(con3[0]+b*con3[1])*(-1)*con3[3]*(1/((con3[2]+a*con3[3])*(con3[2]+a*con3[3])));

 /*Минус*/
 d_daao[0]=(con1[0]+b*con1[1])*(-1)*con1[3]*(1/((con1[2]+ao*con1[3])*(con1[2]+ao*con1[3])));
 d_daao[1]=(con2[0]+b*con2[1])*(-1)*con2[3]*(1/((con2[2]+ao*con2[3])*(con2[2]+ao*con2[3])));
 d_daao[2]=(con3[0]+b*con3[1])*(-1)*con3[3]*(1/((con3[2]+ao*con3[3])*(con3[2]+ao*con3[3])));

 d_dabo[0]=(con1[0]+bo*con1[1])*(-1)*con1[3]*(1/((con1[2]+a*con1[3])*(con1[2]+a*con1[3])));
 d_dabo[1]=(con2[0]+bo*con2[1])*(-1)*con2[3]*(1/((con2[2]+a*con2[3])*(con2[2]+a*con2[3])));
 d_dabo[2]=(con3[0]+bo*con3[1])*(-1)*con3[3]*(1/((con3[2]+a*con3[3])*(con3[2]+a*con3[3])));

 d_daho[0]=(con1[0]+b*con1[1])*(-1)*con1[3]*(1/((con1[2]+a*con1[3])*(con1[2]+a*con1[3])));
 d_daho[1]=(con2[0]+b*con2[1])*(-1)*con2[3]*(1/((con2[2]+a*con2[3])*(con2[2]+a*con2[3])));
 d_daho[2]=(con3[0]+b*con3[1])*(-1)*con3[3]*(1/((con3[2]+a*con3[3])*(con3[2]+a*con3[3])));

 d_db[0]=(1/(con1[2]+a*con1[3]))*con1[1];
 d_db[1]=(1/(con2[2]+a*con2[3]))*con2[1];
 d_db[2]=(1/(con3[2]+a*con3[3]))*con3[1];

 double d_dbap[3],d_dbao[3],d_dbbp[3],d_dbbo[3],d_dbhp[3],d_dbho[3];
 /*Плюс*/
 d_dbap[0]=(1/(con1[2]+ap*con1[3]))*con1[1];
 d_dbap[1]=(1/(con2[2]+ap*con2[3]))*con2[1];
 d_dbap[2]=(1/(con3[2]+ap*con3[3]))*con3[1];

 d_dbbp[0]=(1/(con1[2]+a*con1[3]))*con1[1];
 d_dbbp[1]=(1/(con2[2]+a*con2[3]))*con2[1];
 d_dbbp[2]=(1/(con3[2]+a*con3[3]))*con3[1];

 d_dbhp[0]=(1/(con1[2]+a*con1[3]))*con1[1];
 d_dbhp[1]=(1/(con2[2]+a*con2[3]))*con2[1];
 d_dbhp[2]=(1/(con3[2]+a*con3[3]))*con3[1];
 /*Минус*/
 d_dbao[0]=(1/(con1[2]+ao*con1[3]))*con1[1];
 d_dbao[1]=(1/(con2[2]+ao*con2[3]))*con2[1];
 d_dbao[2]=(1/(con3[2]+ao*con3[3]))*con3[1];

 d_dbbo[0]=(1/(con1[2]+a*con1[3]))*con1[1];
 d_dbbo[1]=(1/(con2[2]+a*con2[3]))*con2[1];
 d_dbbo[2]=(1/(con3[2]+a*con3[3]))*con3[1];

 d_dbho[0]=(1/(con1[2]+a*con1[3]))*con1[1];
 d_dbho[1]=(1/(con2[2]+a*con2[3]))*con2[1];
 d_dbho[2]=(1/(con3[2]+a*con3[3]))*con3[1];


 d_ka[0]=k1*(-2*h*con1[4]);
 d_ka[1]=k2*(-2*h*con2[4]);
 d_ka[2]=k3*(-2*h*con3[4]);

 double d_kaap[3],d_kaao[3],d_kabp[3],d_kabo[3],d_kahp[3],d_kaho[3];
 /*Плюс*/
 d_kaap[0]=kap[0]*(-2*h*con1[4]);
 d_kaap[1]=kap[1]*(-2*h*con2[4]);
 d_kaap[2]=kap[2]*(-2*h*con3[4]);

 d_kabp[0]=kbp[0]*(-2*h*con1[4]);
 d_kabp[1]=kbp[1]*(-2*h*con2[4]);
 d_kabp[2]=kbp[2]*(-2*h*con3[4]);

 d_kahp[0]=khp[0]*(-2*hp*con1[4]);
 d_kahp[1]=khp[1]*(-2*hp*con2[4]);
 d_kahp[2]=khp[2]*(-2*hp*con3[4]);

 /*Минус*/
 d_kaao[0]=kao[0]*(-2*h*con1[4]);
 d_kaao[1]=kao[1]*(-2*h*con2[4]);
 d_kaao[2]=kao[2]*(-2*h*con3[4]);

 d_kabo[0]=kbo[0]*(-2*h*con1[4]);
 d_kabo[1]=kbo[1]*(-2*h*con2[4]);
 d_kabo[2]=kbo[2]*(-2*h*con3[4]);

 d_kaho[0]=kho[0]*(-2*ho*con1[4]);
 d_kaho[1]=kho[1]*(-2*ho*con2[4]);
 d_kaho[2]=kho[2]*(-2*ho*con3[4]);


 d_kb[0]=k1*(-2*h*con1[6]);
 d_kb[1]=k2*(-2*h*con2[6]);
 d_kb[2]=k3*(-2*h*con3[6]);

 double d_kbap[3],d_kbbp[3],d_kbhp[3],d_kbao[3],d_kbbo[3],d_kbho[3];
 /*Плюс*/
 d_kbap[0]=kap[0]*(-2*h*con1[6]);
 d_kbap[1]=kap[1]*(-2*h*con2[6]);
 d_kbap[2]=kap[2]*(-2*h*con3[6]);

 d_kbbp[0]=kbp[0]*(-2*h*con1[6]);
 d_kbbp[1]=kbp[1]*(-2*h*con2[6]);
 d_kbbp[2]=kbp[2]*(-2*h*con3[6]);

 d_kbhp[0]=khp[0]*(-2*hp*con1[6]);
 d_kbhp[1]=khp[1]*(-2*hp*con2[6]);
 d_kbhp[2]=khp[2]*(-2*hp*con3[6]);

 /*Минус*/
 d_kbao[0]=kao[0]*(-2*h*con1[6]);
 d_kbao[1]=kao[1]*(-2*h*con2[6]);
 d_kbao[2]=kao[2]*(-2*h*con3[6]);

 d_kbbo[0]=kbo[0]*(-2*h*con1[6]);
 d_kbbo[1]=kbo[1]*(-2*h*con2[6]);
 d_kbbo[2]=kbo[2]*(-2*h*con3[6]);

 d_kbho[0]=kho[0]*(-2*ho*con1[6]);
 d_kbho[1]=kho[1]*(-2*ho*con2[6]);
 d_kbho[2]=kho[2]*(-2*ho*con3[6]);


 d_kh[0]=k1*(-2*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 d_kh[1]=k2*(-2*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 d_kh[2]=k3*(-2*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

 double d_khap[3],d_khbp[3],d_khhp[3],d_khao[3],d_khbo[3],d_khho[3];
 /*Плюс*/
 d_khap[0]=kap[0]*(-2*(con1[7]+b*con1[6]+con1[5]+ap*con1[4]));
 d_khap[1]=kap[1]*(-2*(con2[7]+b*con2[6]+con2[5]+ap*con2[4]));
 d_khap[2]=kap[2]*(-2*(con3[7]+b*con3[6]+con3[5]+ap*con3[4]));

 d_khbp[0]=kbp[0]*(-2*(con1[7]+bp*con1[6]+con1[5]+a*con1[4]));
 d_khbp[1]=kbp[1]*(-2*(con2[7]+bp*con2[6]+con2[5]+a*con2[4]));
 d_khbp[2]=kbp[2]*(-2*(con3[7]+bp*con3[6]+con3[5]+a*con3[4]));

 d_khhp[0]=khp[0]*(-2*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 d_khhp[1]=khp[0]*(-2*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 d_khhp[2]=khp[0]*(-2*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

 /*Минус*/
 d_khao[0]=kao[0]*(-2*(con1[7]+b*con1[6]+con1[5]+ao*con1[4]));
 d_khao[1]=kao[1]*(-2*(con2[7]+b*con2[6]+con2[5]+ao*con2[4]));
 d_khao[2]=kao[2]*(-2*(con3[7]+b*con3[6]+con3[5]+ao*con3[4]));

 d_khbo[0]=kbo[0]*(-2*(con1[7]+bo*con1[6]+con1[5]+a*con1[4]));
 d_khbo[1]=kbo[1]*(-2*(con2[7]+bo*con2[6]+con2[5]+a*con2[4]));
 d_khbo[2]=kbo[2]*(-2*(con3[7]+bo*con3[6]+con3[5]+a*con3[4]));

 d_khho[0]=kho[0]*(-2*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 d_khho[1]=kho[0]*(-2*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 d_khho[2]=kho[0]*(-2*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));


 f1=(0.2895*d1*(1-k1)+R1b*k1-R1a)*1;
 f2=(0.2895*d2*(1-k2)+R2b*k2-R2a)*1;
 f3=(0.2895*d3*(1-k3)+R3b*k3-R3a)*1;

 double fap[3],fbp[3],fhp[3],fao[3],fbo[3],fho[3];
/*Плюс*/
 fap[0]=(0.2895*dap[0]*(1-kap[0])+R1b*kap[0]-R1a)*1;
 fap[1]=(0.2895*dap[1]*(1-kap[1])+R2b*kap[1]-R2a)*1;
 fap[2]=(0.2895*dap[2]*(1-kap[2])+R3b*kap[2]-R3a)*1;

 fbp[0]=(0.2895*dbp[0]*(1-kbp[0])+R1b*kbp[0]-R1a)*1;
 fbp[1]=(0.2895*dbp[1]*(1-kbp[1])+R2b*kbp[1]-R2a)*1;
 fbp[2]=(0.2895*dbp[2]*(1-kbp[2])+R3b*kbp[2]-R3a)*1;

 fhp[0]=(0.2895*dhp[0]*(1-khp[0])+R1b*khp[0]-R1a)*1;
 fhp[1]=(0.2895*dhp[1]*(1-khp[1])+R2b*khp[1]-R2a)*1;
 fhp[2]=(0.2895*dhp[2]*(1-khp[2])+R3b*khp[2]-R3a)*1;

/*Минус*/
 fao[0]=(0.2895*dao[0]*(1-kao[0])+R1b*kao[0]-R1a)*1;
 fao[1]=(0.2895*dao[1]*(1-kao[1])+R2b*kao[1]-R2a)*1;
 fao[2]=(0.2895*dao[2]*(1-kao[2])+R3b*kao[2]-R3a)*1;

 fbo[0]=(0.2895*dbo[0]*(1-kbo[0])+R1b*kbo[0]-R1a)*1;
 fbo[1]=(0.2895*dbo[1]*(1-kbo[1])+R2b*kbo[1]-R2a)*1;
 fbo[2]=(0.2895*dbo[2]*(1-kbo[2])+R3b*kbo[2]-R3a)*1;

 fho[0]=(0.2895*dho[0]*(1-kho[0])+R1b*kho[0]-R1a)*1;
 fho[1]=(0.2895*dho[1]*(1-kho[1])+R2b*kho[1]-R2a)*1;
 fho[2]=(0.2895*dho[2]*(1-kho[2])+R3b*kho[2]-R3a)*1;


 d_fa[0]=(0.2895*(d_da[0]*(1-k1)+d1*(-1)*d_ka[0])+R1b*d_ka[0])*1;
 d_fa[1]=(0.2895*(d_da[1]*(1-k2)+d2*(-1)*d_ka[1])+R2b*d_ka[1])*1;
 d_fa[2]=(0.2895*(d_da[2]*(1-k3)+d3*(-1)*d_ka[2])+R3b*d_ka[2])*1;

 double d_faap[3],d_fabp[3],d_fahp[3],d_faao[3],d_fabo[3],d_faho[3];
 /*Плюс*/
 d_faap[0]=(0.2895*(d_daap[0]*(1-kap[0])+dap[0]*(-1)*d_kaap[0])+R1b*d_kaap[0])*1;
 d_faap[1]=(0.2895*(d_daap[1]*(1-kap[1])+dap[1]*(-1)*d_kaap[1])+R2b*d_kaap[1])*1;
 d_faap[2]=(0.2895*(d_daap[2]*(1-kap[2])+dap[2]*(-1)*d_kaap[2])+R3b*d_kaap[2])*1;

 d_fabp[0]=(0.2895*(d_dabp[0]*(1-kbp[0])+dbp[0]*(-1)*d_kabp[0])+R1b*d_kabp[0])*1;
 d_fabp[1]=(0.2895*(d_dabp[1]*(1-kbp[1])+dbp[1]*(-1)*d_kabp[1])+R2b*d_kabp[1])*1;
 d_fabp[2]=(0.2895*(d_dabp[2]*(1-kbp[2])+dbp[2]*(-1)*d_kabp[2])+R3b*d_kabp[2])*1;

 d_fahp[0]=(0.2895*(d_dahp[0]*(1-khp[0])+dhp[0]*(-1)*d_kahp[0])+R1b*d_kahp[0])*1;
 d_fahp[1]=(0.2895*(d_dahp[1]*(1-khp[1])+dhp[1]*(-1)*d_kahp[1])+R2b*d_kahp[1])*1;
 d_fahp[2]=(0.2895*(d_dahp[2]*(1-khp[2])+dhp[2]*(-1)*d_kahp[2])+R3b*d_kahp[2])*1;
 /*Минус*/
 d_faao[0]=(0.2895*(d_daao[0]*(1-kao[0])+dao[0]*(-1)*d_kaao[0])+R1b*d_kaao[0])*1;
 d_faao[1]=(0.2895*(d_daao[1]*(1-kao[1])+dao[1]*(-1)*d_kaao[1])+R2b*d_kaao[1])*1;
 d_faao[2]=(0.2895*(d_daao[2]*(1-kao[2])+dao[2]*(-1)*d_kaao[2])+R3b*d_kaao[2])*1;

 d_fabo[0]=(0.2895*(d_dabo[0]*(1-kbo[0])+dbo[0]*(-1)*d_kabo[0])+R1b*d_kabo[0])*1;
 d_fabo[1]=(0.2895*(d_dabo[1]*(1-kbo[1])+dbo[1]*(-1)*d_kabo[1])+R2b*d_kabo[1])*1;
 d_fabo[2]=(0.2895*(d_dabo[2]*(1-kbo[2])+dbo[2]*(-1)*d_kabo[2])+R3b*d_kabo[2])*1;

 d_faho[0]=(0.2895*(d_daho[0]*(1-kho[0])+dho[0]*(-1)*d_kaho[0])+R1b*d_kaho[0])*1;
 d_faho[1]=(0.2895*(d_daho[1]*(1-kho[1])+dho[1]*(-1)*d_kaho[1])+R2b*d_kaho[1])*1;
 d_faho[2]=(0.2895*(d_daho[2]*(1-kho[2])+dho[2]*(-1)*d_kaho[2])+R3b*d_kaho[2])*1;


 d_fb[0]=(0.2895*(d_db[0]*(1-k1)+d1*(-1)*d_kb[0])+R1b*d_kb[0])*1;
 d_fb[1]=(0.2895*(d_db[1]*(1-k2)+d2*(-1)*d_kb[1])+R2b*d_kb[1])*1;
 d_fb[2]=(0.2895*(d_db[2]*(1-k3)+d3*(-1)*d_kb[2])+R3b*d_kb[2])*1;

 double d_fbap[3],d_fbbp[3],d_fbhp[3],d_fbao[3],d_fbbo[3],d_fbho[3];
 /*Плюс*/
 d_fbap[0]=(0.2895*(d_dbap[0]*(1-kap[0])+dap[0]*(-1)*d_kbap[0])+R1b*d_kbap[0])*1;
 d_fbap[1]=(0.2895*(d_dbap[1]*(1-kap[1])+dap[1]*(-1)*d_kbap[1])+R2b*d_kbap[1])*1;
 d_fbap[2]=(0.2895*(d_dbap[2]*(1-kap[2])+dap[2]*(-1)*d_kbap[2])+R3b*d_kbap[2])*1;

 d_fbbp[0]=(0.2895*(d_dbbp[0]*(1-kbp[0])+dbp[0]*(-1)*d_kbbp[0])+R1b*d_kbbp[0])*1;
 d_fbbp[1]=(0.2895*(d_dbbp[1]*(1-kbp[1])+dbp[1]*(-1)*d_kbbp[1])+R2b*d_kbbp[1])*1;
 d_fbbp[2]=(0.2895*(d_dbbp[2]*(1-kbp[2])+dbp[2]*(-1)*d_kbbp[2])+R3b*d_kbbp[2])*1;

 d_fbhp[0]=(0.2895*(d_dbhp[0]*(1-khp[0])+dhp[0]*(-1)*d_kbhp[0])+R1b*d_kbhp[0])*1;
 d_fbhp[1]=(0.2895*(d_dbhp[1]*(1-khp[1])+dhp[1]*(-1)*d_kbhp[1])+R2b*d_kbhp[1])*1;
 d_fbhp[2]=(0.2895*(d_dbhp[2]*(1-khp[2])+dhp[2]*(-1)*d_kbhp[2])+R3b*d_kbhp[2])*1;

 /*Минус*/
 d_fbao[0]=(0.2895*(d_dbao[0]*(1-kao[0])+dao[0]*(-1)*d_kbao[0])+R1b*d_kbao[0])*1;
 d_fbao[1]=(0.2895*(d_dbao[1]*(1-kao[1])+dao[1]*(-1)*d_kbao[1])+R2b*d_kbao[1])*1;
 d_fbao[2]=(0.2895*(d_dbao[2]*(1-kao[2])+dao[2]*(-1)*d_kbao[2])+R3b*d_kbao[2])*1;

 d_fbbo[0]=(0.2895*(d_dbbo[0]*(1-kbo[0])+dbo[0]*(-1)*d_kbbo[0])+R1b*d_kbbo[0])*1;
 d_fbbo[1]=(0.2895*(d_dbbo[1]*(1-kbo[1])+dbo[1]*(-1)*d_kbbo[1])+R2b*d_kbbo[1])*1;
 d_fbbo[2]=(0.2895*(d_dbbo[2]*(1-kbo[2])+dbo[2]*(-1)*d_kbbo[2])+R3b*d_kbbo[2])*1;

 d_fbho[0]=(0.2895*(d_dbho[0]*(1-kho[0])+dho[0]*(-1)*d_kbho[0])+R1b*d_kbho[0])*1;
 d_fbho[1]=(0.2895*(d_dbho[1]*(1-kho[1])+dho[1]*(-1)*d_kbho[1])+R2b*d_kbho[1])*1;
 d_fbho[2]=(0.2895*(d_dbho[2]*(1-kho[2])+dho[2]*(-1)*d_kbho[2])+R3b*d_kbho[2])*1;


 d_fh[0]=(0.2895*d1*(-1)*d_kh[0]+R1b*d_kh[0])*1;
 d_fh[1]=(0.2895*d2*(-1)*d_kh[1]+R2b*d_kh[1])*1;
 d_fh[2]=(0.2895*d3*(-1)*d_kh[2]+R3b*d_kh[2])*1;

 double d_fhap[3],d_fhbp[3],d_fhhp[3],d_fhao[3],d_fhbo[3],d_fhho[3];
 /*Плюс*/
 d_fhap[0]=(0.2895*dap[0]*(-1)*d_khap[0]+R1b*d_khap[0])*1;
 d_fhap[1]=(0.2895*dap[1]*(-1)*d_khap[1]+R2b*d_khap[1])*1;
 d_fhap[2]=(0.2895*dap[2]*(-1)*d_khap[2]+R3b*d_khap[2])*1;

 d_fhbp[0]=(0.2895*dbp[0]*(-1)*d_khbp[0]+R1b*d_khbp[0])*1;
 d_fhbp[1]=(0.2895*dbp[1]*(-1)*d_khbp[1]+R2b*d_khbp[1])*1;
 d_fhbp[2]=(0.2895*dbp[2]*(-1)*d_khbp[2]+R3b*d_khbp[2])*1;

 d_fhhp[0]=(0.2895*dhp[0]*(-1)*d_khhp[0]+R1b*d_khhp[0])*1;
 d_fhhp[1]=(0.2895*dhp[0]*(-1)*d_khhp[1]+R2b*d_khhp[1])*1;
 d_fhhp[2]=(0.2895*dhp[0]*(-1)*d_khhp[2]+R3b*d_khhp[2])*1;
 /*Минус*/
 d_fhao[0]=(0.2895*dao[0]*(-1)*d_khao[0]+R1b*d_khao[0])*1;
 d_fhao[1]=(0.2895*dao[1]*(-1)*d_khao[1]+R2b*d_khao[1])*1;
 d_fhao[2]=(0.2895*dao[2]*(-1)*d_khao[2]+R3b*d_khao[2])*1;

 d_fhbo[0]=(0.2895*dbo[0]*(-1)*d_khbo[0]+R1b*d_khbo[0])*1;
 d_fhbo[1]=(0.2895*dbo[1]*(-1)*d_khbo[1]+R2b*d_khbo[1])*1;
 d_fhbo[2]=(0.2895*dbo[2]*(-1)*d_khbo[2]+R3b*d_khbo[2])*1;

 d_fhho[0]=(0.2895*dho[0]*(-1)*d_khho[0]+R1b*d_khho[0])*1;
 d_fhho[1]=(0.2895*dho[0]*(-1)*d_khho[1]+R2b*d_khho[1])*1;
 d_fhho[2]=(0.2895*dho[0]*(-1)*d_khho[2]+R3b*d_khho[2])*1;

 d_FMainA=2*f1*d_fa[0]+2*f2*d_fa[1]+2*f3*d_fa[2];
 d_FMainB=2*f1*d_fb[0]+2*f2*d_fb[1]+2*f3*d_fb[2];
 d_FMainH=2*f1*d_fh[0]+2*f2*d_fh[1]+2*f3*d_fh[2];

 double d_FMain[3];
 d_FMain[0]=d_FMainA;
 d_FMain[1]=d_FMainB;
 d_FMain[2]=d_FMainH;


 double d_FMainLen;
 d_FMainLen=sqrt(d_FMainA*d_FMainA+d_FMainB*d_FMainB+d_FMainH*d_FMainH);

 double d2_FMainP[9],d2_FMainO[9];
 /*Плюс*/
 d2_FMainP[0]=2*fap[0]*d_faap[0]+2*fap[1]*d_faap[1]+2*fap[2]*d_faap[2];
 d2_FMainP[1]=2*fbp[0]*d_fabp[0]+2*fbp[1]*d_fabp[1]+2*fbp[2]*d_fabp[2];
 d2_FMainP[2]=2*fhp[0]*d_fahp[0]+2*fhp[1]*d_fahp[1]+2*fhp[2]*d_fahp[2];

 d2_FMainP[3]=2*fap[0]*d_fbap[0]+2*fap[1]*d_fbap[1]+2*fap[2]*d_fbap[2];
 d2_FMainP[4]=2*fbp[0]*d_fbbp[0]+2*fbp[1]*d_fbbp[1]+2*fbp[2]*d_fbbp[2];
 d2_FMainP[5]=2*fhp[0]*d_fbhp[0]+2*fhp[1]*d_fbhp[1]+2*fhp[2]*d_fbhp[2];

 d2_FMainP[6]=2*fap[0]*d_fhap[0]+2*fap[1]*d_fhap[1]+2*fap[2]*d_fhap[2];
 d2_FMainP[7]=2*fbp[0]*d_fhbp[0]+2*fbp[1]*d_fhbp[1]+2*fbp[2]*d_fhbp[2];
 d2_FMainP[8]=2*fhp[0]*d_fhhp[0]+2*fhp[1]*d_fhhp[1]+2*fhp[2]*d_fhhp[2];
 /*Минус*/
 d2_FMainO[0]=2*fao[0]*d_faao[0]+2*fao[1]*d_faao[1]+2*fao[2]*d_faao[2];
 d2_FMainO[1]=2*fbo[0]*d_fabo[0]+2*fbo[1]*d_fabo[1]+2*fbo[2]*d_fabo[2];
 d2_FMainO[2]=2*fho[0]*d_faho[0]+2*fho[1]*d_faho[1]+2*fho[2]*d_faho[2];

 d2_FMainO[3]=2*fao[0]*d_fbao[0]+2*fao[1]*d_fbao[1]+2*fao[2]*d_fbao[2];
 d2_FMainO[4]=2*fbo[0]*d_fbbo[0]+2*fbo[1]*d_fbbo[1]+2*fbo[2]*d_fbbo[2];
 d2_FMainO[5]=2*fho[0]*d_fbho[0]+2*fho[1]*d_fbho[1]+2*fho[2]*d_fbho[2];

 d2_FMainO[6]=2*fao[0]*d_fhao[0]+2*fao[1]*d_fhao[1]+2*fao[2]*d_fhao[2];
 d2_FMainO[7]=2*fbo[0]*d_fhbo[0]+2*fbo[1]*d_fhbo[1]+2*fbo[2]*d_fhbo[2];
 d2_FMainO[8]=2*fho[0]*d_fhho[0]+2*fho[1]*d_fhho[1]+2*fho[2]*d_fhho[2];

 double d2_FMain[9];

 for(int i=0;i<9;i++)
  d2_FMain[i]=(d2_FMainP[i]-d2_FMainO[i])/(2*dh);

 FMain=f1*f1+f2*f2+f3*f3;




 double *dxy;
 double *NUI;
 double diskr;
 DNMathAdd Math1;

 /*nu*I*/
 NUI=Math1.MatrMulti(I,3,3,0.5);
 /*At*A+nu*I*/
 Math1.MatrAdd(d2_FMain,NUI,3,3);

 diskr=Math1.CalcOpredMatr(3,d2_FMain);
 Math1.MatrInverse(d2_FMain,3);

 for(int i=0;i<3*3;i++)
  d2_FMain[i]*=-1;

 dxy=Math1.MatrMulti(d2_FMain,3,3,d_FMain,1,3);

 a+=dxy[0];
 b+=dxy[1];
 h+=dxy[2];

 d1=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
 d2=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
 d3=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);
 k1=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 k2=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 k3=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));
 f1=(0.2895*d1*(1-k1)+R1b*k1-R1a)*1;
 f2=(0.2895*d2*(1-k2)+R2b*k2-R2a)*1;
 f3=(0.2895*d3*(1-k3)+R3b*k3-R3a)*1;
 double FMain2;
 FMain2=f1*f1+f2*f2+f3*f3;
 dFMain=fabs(FMain2-FMain);



// QMessageBox msg;
// msg.setText(QString().setNum(dxy[0],'d',4)+"  "+
//             QString().setNum(dxy[1],'d',4)+"  "+
//             QString().setNum(dxy[2],'d',4)+"\n"+
//             QString().setNum(FMain,'d',4)+"  "+
//             QString().setNum(FMain2,'d',4)+"\n"+
//             QString().setNum(dFMain,'d',4));
// msg.exec();
//  QMessageBox msg;
//  msg.setText(QString().setNum(a,'d',5)+"  "+
//              QString().setNum(b,'d',5)+"  "+
//              QString().setNum(h,'d',5)+"\n"+
//              QString().setNum(dFMain,'d',5)+"\n"+
//              QString().setNum(FMain2,'d',5));
//  msg.exec();

  delete[] dxy;
  delete[] NUI;

  if(dFMain<1.e-10)
  {
   FMain=FMain2;
   break;
  }
  if(dFMain>=1.e5)
  {
   FMain=FMain2;
   break;
  }
 }while(/*dFMain>0.0000001*/true);/*while(delta[0]*delta[0]+delta[1]*delta[1]+delta[2]*delta[2]>0.00000001);*/




 ReturnRes.a=a;
 ReturnRes.b=b;
 ReturnRes.h=h;
 ReturnRes.FMin=FMain;
 return ReturnRes;
}

BatimetrResult DNTheam::CalcFunkBatimetr2(double a, double b, double h, float R1a, float R2a, float R3a,double R1b,double R2b,double R3b)
{
 double f1,f2,f3;
 double d1,d2,d3,k1,k2,k3;
 double lamda1=425.,lamda2=545.,lamda3=605.;
 //double awLamda1=0.0045,awLamda2=0.056,awLamda3=0.240;  //показатель поглощения чистой морской водой
 double awLamda1=0.015,awLamda2=0.06,awLamda3=0.245;  //показатель поглощения чистой морской водой
 //double bwLamda1=0.004,bwLamda2=0.00135,bwLamda3=0.00093;  //показатель рассеивания чистой морской водой
 double bwLamda1=0.004,bwLamda2=0.0018,bwLamda3=0.00115;  //показатель рассеивания чистой морской водой
 double ALamda1=0.0362,ALamda2=0.009,ALamda3=0.0056; //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double BLamda1=0.313,BLamda2=0.073,BLamda3=0.093;  //коэффициенты для расчета покмзмтеля поглощения хлорофиллом
 double s500=0.017,s501=0.011; //коэффициенты для расчета показателя поглощения РОВ
 double Cph=0.6; //концентрация хлорофилла мг/м3

 double F,hMain=-1;
 double con1[8],con2[8],con3[8];


 BatimetrResult ReturnRes;

 con1[0]=bwLamda1;
 con1[1]=pow((555/lamda1),(-1));
 con1[2]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[3]=exp(-s500*(lamda1-443));

 con1[4]=exp(-s500*(lamda1-443));
 con1[5]=awLamda1+Cph*ALamda1*pow(Cph,(-BLamda1));
 con1[6]=pow((555/lamda1),(-1));
 con1[7]=bwLamda1;

 con2[0]=bwLamda2;
 con2[1]=pow((555/lamda2),-1);
 con2[2]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[3]=exp(-s501*(lamda2-443));

 con2[4]=exp(-s501*(lamda2-443));
 con2[5]=awLamda2+Cph*ALamda2*pow(Cph,(-BLamda2));
 con2[6]=pow((555/lamda2),(-1));
 con2[7]=bwLamda2;

 con3[0]=bwLamda3;
 con3[1]=pow((555/lamda3),(-1));
 con3[2]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[3]=exp(-s501*(lamda3-443));

 con3[4]=exp(-s501*(lamda3-443));
 con3[5]=awLamda3+Cph*ALamda3*pow(Cph,(-BLamda3));
 con3[6]=pow((555/lamda3),(-1));
 con3[7]=bwLamda3;

 hMain=-5;

 // Описание исходных уравнений

 d1=(con1[0]+b*con1[1])/(con1[2]+a*con1[3]);
 d2=(con2[0]+b*con2[1])/(con2[2]+a*con2[3]);
 d3=(con3[0]+b*con3[1])/(con3[2]+a*con3[3]);

 k1=exp(-2*h*(con1[7]+b*con1[6]+con1[5]+a*con1[4]));
 k2=exp(-2*h*(con2[7]+b*con2[6]+con2[5]+a*con2[4]));
 k3=exp(-2*h*(con3[7]+b*con3[6]+con3[5]+a*con3[4]));

 f1=2.895*d1-2.895*d1*k1+R1b*k1-R1a;
 f2=2.895*d2-2.895*d2*k2+R2b*k2-R2a;
 f3=2.895*d3-2.895*d3*k3+R3b*k3-R3a;

 F=pow((f1*f1+f2*f2+f3*f3),0.5);

 ReturnRes.a=a;
 ReturnRes.b=b;
 ReturnRes.h=h;
 ReturnRes.FMin=F;

 return ReturnRes;
}

double DNTheam::CalcFunkBatimetrNew(int TypeBottom,float R485, float R560,float R660,float *Ra)
{
 double a440,a485;
 double step;
 double a_lam[3];
 double b_lam[3];
 double k_lam[3];
 double Rdp[3];
 double Rb[3];
 double H_lam[3];
 double H_main;

 double b_bp660;

 double ProbRa[3];

 const double aw440=0.00635;
 const double aw485=0.0136;
 const double aw660=0.41;
 const double M440=1.5;
 const double aw[3]={0.00478,0.0511,0.2224};
 const double bw[3]={0.00581,0.00193,0.00141};
 const double lam[3]={425.,545.,605.};
 const double M[3]={1.6108,0.6094,0.509};

 TypeBottom=1;

 if(TypeBottom==0) //Макрофиты
 {
  Rb[0]=0.025;
  Rb[1]=0.1;
  Rb[2]=0.13;
 }
 if(TypeBottom==1)  //Песок
 {
  Rb[0]=0.1;
  Rb[1]=0.18;
  Rb[2]=0.22;
 }
 if(TypeBottom==2) //Водоросли
 {
  Rb[0]=0.025;
  Rb[1]=0.07;
  Rb[2]=0.1;
 }

 ProbRa[0]=Ra[0];
 ProbRa[1]=Ra[1];
 ProbRa[2]=Ra[2];
 double proba;
 proba=Ra[0];
 proba=Ra[1];
 proba=Ra[2];
 step=-0.619-1.969*log10(R485/R560)+0.79*log10(R485/R560)*log10(R485/R560);
 proba=step;
 a440=pow(10,step);
 a485=((a440-aw440)/M440)+aw485;
 for(int i=0;i<3;i++)
  a_lam[i]=M[i]*(a485-aw485)+aw[i];

 b_bp660=(aw660/0.05)*R660;
 H_main=0;
 bool IsDeepBig=false;
 for(int i=0;i<3;i++)
 {
  if((Ra[i]-Rdp[i])/(Rb[i]-Rdp[i])<0)
   IsDeepBig=true;
 }
// double Znam;
// double Chisl;
 int sch=0;
 for(int i=0;i<3;i++)
 {
  proba=Ra[i];
  b_lam[i]=b_bp660*pow(660/lam[i],0.5)+bw[i];
  Rdp[i]=0.05*b_lam[i]/(a_lam[i]+b_lam[i]);
  k_lam[i]=a_lam[i]+b_lam[i];

//  if(!IsDeepBig)
//   Znam=(Rb[i]-Rdp[i]);
//  else
//   Znam=-Rdp[i];
//  Chisl=Ra[i]-Rdp[i];

//  if(Chisl/Znam>0 && k_lam[i]!=0)
//  {
   H_lam[i]=this->CalcFunkNew(Rdp[i],Ra[i],Rb[i],k_lam[i]);/*log(Chisl/Znam)/(-2*k_lam[i]);*/
   H_main+=H_lam[i];
   sch++;
//  }
 }
 if(sch>0)
  H_main=H_main/sch;
 if(sch==0 || H_main>10.)
  H_main=10;
 return H_main;
}

double DNTheam::CalcFunkNew(double Rdp,double Ra,double Rb,double k)
{
 double Funk,dFunk,Fp,Fo;
 double h=0.1,hp,ho;
 const double delta=0.00001;
 int den;

 if(Ra<Rdp)
 {
  h=10;
  return h;
 }
 if(Ra>Rb)
 {
  h=0.1;
  return h;
 }
 for(int i=0;i<20;i++)
 {

  Funk=Rdp*(1-exp(-3.2*k*h))+Rb*exp(-2.7*k*h)-Ra;
  hp=h+delta;
  ho=h-delta;
  Fp=Rdp*(1-exp(-3.2*k*hp))+Rb*exp(-2.7*k*hp)-Ra;
  Fo=Rdp*(1-exp(-3.2*k*ho))+Rb*exp(-2.7*k*ho)-Ra;
  dFunk=(Fp-Fo)/(2*delta);
  h-=Funk/dFunk;
  Funk=Rdp*(1-exp(-3.2*k*h))+Rb*exp(-2.7*k*h)-Ra;

  if(Funk<0.001)
   break;
 }
 return h;
}
