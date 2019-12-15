#ifndef ONLINE_DISPLAY_HMS_H
#define ONLINE_DISPLAY_HMS_H

R__LOAD_LIBRARY(libScandalizer.so)
#include "scandalizer/PostProcessors.h"
#include "scandalizer/ScriptHelpers.h"
#include "scandalizer/SpectrometerMonitor.h"
#include "monitor/DetectorDisplay.h"
#include "monitor/EventDisplays.h"
#include "monitor/ExperimentMonitor.h"


void find_and_plot1(hallc::DisplayPlot &plt, const char *name, int canvas_id, int color = 1, const char *draw_opt = "", bool save = false)
{
    auto th = (TH1F*) gROOT->FindObject(name);
    if (th == nullptr) {
        std::cout << "Failed to find hist " << name << "\n";
    } else {
        plt._plot_data._canvas->cd(canvas_id);
        if (save) { plt._plot_data._hists1.push_back(th); }
        th->SetLineColor(color);
        th->Draw(draw_opt);
    }
}

void find_and_plot2(hallc::DisplayPlot &plt, const char *name, int canvas_id, const char *draw_opt = "colz", bool save = true)
{
    auto th = (TH2F*) gROOT->FindObject(name);
    if (th == nullptr) {
        std::cout << "Failed to find hist " << name << "\n";
    } else {
        plt._plot_data._canvas->cd(canvas_id);
        if (save) { plt._plot_data._hists2.push_back(th); }
        th->Draw(draw_opt);
    }
}

hallc::MonitoringDisplay *CreateMonitoringDisplay(const char *root_dir)
{
    gROOT->SetBatch();
    auto display = new hallc::MonitoringDisplay();
    display->SetRootFolder(root_dir);

    // target
    display->CreateDisplayPlot(
        "target/", "HMS Target Variables",
        [] (hallc::DisplayPlot &plt) {
            plt._plot_data._canvas = new TCanvas("hms_tar_vars", "HMS Target Variables");
            plt._plot_data._canvas->Divide(3, 2);
            find_and_plot2(plt, "hgtr_ytar_vs_yptar", 1);
            find_and_plot2(plt, "hgtr_xptar_vs_yptar", 2);
            find_and_plot2(plt, "hgtr_xtar_vs_reactz", 3);
            find_and_plot1(plt, "hgtr_xtar", 4);
            find_and_plot1(plt, "hgtr_ytar", 5);
            find_and_plot2(plt, "hextcor_xysieve", 6);
	        return 0;
        },
        [] (hallc::DisplayPlot &plt) { return 0; }
    );

    // kinematics
    display->CreateDisplayPlot(
        "kinematics/", "HMS Kinematic Variables",
        [] (hallc::DisplayPlot &plt) {
            plt._plot_data._canvas = new TCanvas("hms_kin_vars", "HMS Kinematic Variables");
            plt._plot_data._canvas->Divide(2, 2);
            find_and_plot1(plt, "hkin_q2", 1);
            find_and_plot1(plt, "hkin_omega", 2);
            find_and_plot1(plt, "hkin_w", 3);
            find_and_plot1(plt, "hkin_theta", 4);
            return 0;
        },
        [] (hallc::DisplayPlot &plt) { return 0; }
    );

    gROOT->SetBatch(false);
    return display;
}

#endif // ONLINE_DISPLAY_HMS_H
