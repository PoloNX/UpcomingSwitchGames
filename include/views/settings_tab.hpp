#pragma once

#include <borealis.hpp>

class SettingsTab : public brls::Box {
    public:
        SettingsTab();

    private:
        void on_titledb_selector_change();
        void on_language_selector_change();

        BRLS_BIND(brls::SelectorCell, titledb_selector, "titledb_selector");
        BRLS_BIND(brls::SelectorCell, language_selector, "language_selector");
};