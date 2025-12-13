#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/progdlg.h>

#include <vector>
#include <string>
#include <algorithm>
#include <random>

// ----- Структуры игры -----
struct InsurancePolicy {
    std::string name;
    double premium;             // monthly
    int duration_months;
    double max_payout;
    double franchise;
    int valid_until_month;
};

struct InsuranceType {
    std::string name;
    int base_demand;
    int current_demand;
    int sold_this_month;
    InsurancePolicy policy;
};

class InsuranceGame {
public:
    double capital = 30000.0;
    double tax_rate = 0.09;
    int current_month = 0;
    const int total_months = 12;
    bool is_over = false;

    std::vector<InsuranceType> insurance_types = {
        {"Property", 100, 0, 0, {"Property", 8.0, 12, 10000, 500, 12}},
        {"Auto", 80, 0, 0, {"Auto", 12.0, 6, 15000, 1000, 6}},
        {"Health", 120, 0, 0, {"Health", 10.0, 3, 8000, 300, 3}}
    };

    // Вспомогательные функции
    int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    double randomDouble(double min, double max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    bool isBankrupt() const {
        return capital <= 0;
    }

    wxString payTax() {
        double tax = capital * tax_rate;
        capital -= tax;
        return wxString::Format("Tax paid: %.2f", tax);
    }

    wxString updateDemands() {
        wxString log;
        for (auto& type : insurance_types) {
            const auto& policy = type.policy;
            double total_cost = policy.premium * policy.duration_months;
            double ratio = total_cost / policy.max_payout;
            double multiplier = 1.0 / (1.0 + ratio);
            multiplier = std::clamp(multiplier, 0.5, 2.0);
            type.current_demand = std::max(0, static_cast<int>(
                type.base_demand * multiplier + randomInt(-10, 10)
                ));
            log += wxString::Format("Demand for '%s': %d\n", type.name, type.current_demand);
        }
        return log;
    }

    wxString sellPolicies() {
        wxString log;
        for (auto& type : insurance_types) {
            int sold = std::min(type.current_demand, 1000);
            type.sold_this_month = sold;
            double income = sold * type.policy.premium * type.policy.duration_months;
            capital += income;
            log += wxString::Format("Sold '%s': %d, income: %.2f\n", type.name, sold, income);
        }
        return log;
    }

    wxString processClaims() {
        wxString log;
        for (auto& type : insurance_types) {
            int num_claims = randomInt(1, 25);
            double total_payout = 0.0;
            for (int i = 0; i < num_claims; ++i) {
                double damage_ratio = randomDouble(0.0, 1.0);
                double damage = damage_ratio * type.policy.max_payout;
                if (damage <= type.policy.franchise) continue;
                total_payout += std::min(damage, type.policy.max_payout);
            }
            capital -= total_payout;
            log += wxString::Format("Claims paid for '%s': %.2f\n", type.name, total_payout);
        }
        return log;
    }

    wxString printStatus() {
        wxString log = wxString::Format("\n=== Status at end of month %d ===\n", current_month);
        log += wxString::Format("Capital: %.2f\n", capital);
        for (const auto& type : insurance_types) {
            log += wxString::Format("%s: sold %d, demand %d\n",
                type.name, type.sold_this_month, type.current_demand);
        }
        return log;
    }

    bool needsPolicyUpdate() {
        for (const auto& type : insurance_types) {
            if (current_month >= type.policy.valid_until_month) {
                return true;
            }
        }
        return false;
    }

    InsuranceType* findExpiringPolicy() {
        for (auto& type : insurance_types) {
            if (current_month >= type.policy.valid_until_month) {
                return &type;
            }
        }
        return nullptr;
    }

    wxString setNewPolicy(const wxString& name, double premium, int duration,
        double max_payout, double franchise) {
        for (auto& type : insurance_types) {
            if (type.name == name.ToStdString()) {
                type.policy.premium = premium;
                type.policy.duration_months = duration;
                type.policy.max_payout = max_payout;
                type.policy.franchise = franchise;
                type.policy.valid_until_month = current_month + duration;
                return wxString::Format("Policy for '%s' updated until month %d",
                    name, type.policy.valid_until_month);
            }
        }
        return "Error: policy not found";
    }
};

// ----- GUI -----
class GameFrame : public wxFrame {
public:
    GameFrame() : wxFrame(nullptr, wxID_ANY, "Insurance Business Simulator") {
        m_game = std::make_unique<InsuranceGame>();

        // Панель и лог
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        m_log = new wxTextCtrl(this, wxID_ANY, "",
            wxDefaultPosition, wxSize(-1, 350),
            wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2);
        m_nextBtn = new wxButton(this, wxID_ANY, "Next Month");

        mainSizer->Add(m_log, 1, wxEXPAND | wxALL, 5);
        mainSizer->Add(m_nextBtn, 0, wxALIGN_CENTER | wxALL, 5);
        SetSizer(mainSizer);

        // Приветствие
        m_log->AppendText("=== Insurance Business Simulator ===\n");
        m_log->AppendText(wxString::Format("Starting capital: %.2f\n", m_game->capital));

        // Связи
        Bind(wxEVT_BUTTON, &GameFrame::OnNextMonth, this, m_nextBtn->GetId());
    }

private:
    std::unique_ptr<InsuranceGame> m_game;
    wxTextCtrl* m_log;
    wxButton* m_nextBtn;

    void OnNextMonth(wxCommandEvent& event) {
        if (m_game->is_over) return;

        m_game->current_month++;
        m_log->AppendText(wxString::Format("\n--- Month %d ---\n", m_game->current_month));

        // Шаг 1: налог
        m_log->AppendText(m_game->payTax() + "\n");
        if (m_game->isBankrupt()) {
            m_log->AppendText("Company is bankrupt!\n");
            m_nextBtn->Enable(false);
            m_game->is_over = true;
            return;
        }

        // Шаг 2: спрос и продажи
        m_log->AppendText("Updating demand...\n");
        m_log->AppendText(m_game->updateDemands());
        m_log->AppendText("Selling policies...\n");
        m_log->AppendText(m_game->sellPolicies());
        if (m_game->isBankrupt()) {
            m_log->AppendText("Company is bankrupt!\n");
            m_nextBtn->Enable(false);
            m_game->is_over = true;
            return;
        }

        // Шаг 3: страховые выплаты
        m_log->AppendText("Processing claims...\n");
        m_log->AppendText(m_game->processClaims());
        if (m_game->isBankrupt()) {
            m_log->AppendText("Company is bankrupt!\n");
            m_nextBtn->Enable(false);
            m_game->is_over = true;
            return;
        }

        // Шаг 4: обновление условий, если нужно
        if (m_game->needsPolicyUpdate()) {
            handlePolicyUpdate();
        }

        // Статус
        m_log->AppendText(m_game->printStatus());

        // Конец игры?
        if (m_game->current_month >= m_game->total_months) {
            m_log->AppendText(wxString::Format("\nGame finished! Final capital: %.2f\n", m_game->capital));
            m_nextBtn->Enable(false);
            m_game->is_over = true;
        }
    }

    void handlePolicyUpdate() {
        InsuranceType* expiring = m_game->findExpiringPolicy();
        if (!expiring) return;

        wxString msg = wxString::Format(
            "Policy for '%s' has expired!\n"
            "Current: premium=%.2f, duration=%d months, payout=%.0f, franchise=%.0f\n"
            "Enter NEW values (premium duration payout franchise):",
            expiring->name,
            expiring->policy.premium,
            expiring->policy.duration_months,
            expiring->policy.max_payout,
            expiring->policy.franchise
        );

        wxTextEntryDialog dlg(this, msg, "Update Policy");
        if (dlg.ShowModal() == wxID_OK) {
            wxString input = dlg.GetValue();
            std::vector<wxString> parts = wxStringTokenize(input, " \t");
            if (parts.size() == 4) {
                double premium = wxAtof(parts[0]);
                long duration, payout, franchise;
                if (parts[1].ToLong(&duration) && parts[2].ToLong(&payout) && parts[3].ToLong(&franchise)) {
                    wxString result = m_game->setNewPolicy(
                        expiring->name,
                        premium,
                        (int)duration,
                        (double)payout,
                        (double)franchise
                    );
                    m_log->AppendText(result + "\n");
                    return;
                }
            }
        }
        m_log->AppendText("Failed to update policy. Using old values.\n");
    }
};

// ----- Точка входа -----
class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        GameFrame* frame = new GameFrame();
        frame->SetSize(600, 500);
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);