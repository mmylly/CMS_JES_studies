

{
TGraphErrors* g= new TGraphErrors("jecdataGH.root", "%lg %lg %lg", ",");
Double_t *x=g->GetX();
Double_t *y=g->GetY();
Double_t *ex=g->GetEX();

g->Print();

//printf("%g %g %g\n",x[0],y[0],ex[0]);

//for (int i=0; i<6; i++) printf("%g %g %g\n",x[i],y[i],ex[i]);
}
