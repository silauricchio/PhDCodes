void fit_D2_cuts()
{
  gStyle->SetOptFit(1);

  float X[12]={0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55};
  float Y[12]={2.8, 1.5, 1.2, 1.1, 1.0, 1.0, 0.8, 0.7, 0.7, 0.6, 0.6};
  float Y_err[12]={0.45, 0.10, 0.07, 0.10, 0.07, 0.09, 0.07, 0.11, 0.10, 0.09, 0.07};
  const int n = 11;

  TGraphErrors *graph = new TGraphErrors(n, X, Y, 0, Y_err);
  
  graph->Draw("AP");
  graph->SetMarkerStyle(8);
  graph->SetMarkerColor(kViolet);
  graph->SetLineColor(kViolet);
  graph->GetXaxis()->SetTitle("mX/pTX");
  graph->GetYaxis()->SetTitle("D2X cut");
  
  TF1 *pol = new TF1("pol", "expo");
  graph->Fit("pol");

  pol->SetLineColor(kRed);
  pol->Draw("same");

  TLatex l;
  l.SetTextFont(72);
  l.SetTextSize(0.035);
  l.DrawLatexNDC(0.14, 0.85, "ATLAS");
  l.SetTextFont(42);
  l.DrawLatexNDC(0.22, 0.85, "Internal");
  l.DrawLatexNDC(0.14, 0.8, TString::Format("#scale[0.8]{#sqrt{s} = 13 TeV,   #int Ldt = %.1f fb^{-1}}", 139.));

  

}
