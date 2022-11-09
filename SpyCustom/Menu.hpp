#pragma once



IMaterial* CreateMaterial(std::string matname, std::string mat_data = "")
{
    KeyValues* keyValues = new KeyValues(matname.c_str());
    keyValues->Init();
    keyValues->LoadFromBuffer(matname.c_str(), mat_data.c_str());
    IMaterial* newmat = iff.g_pMaterialSystem->CreateMaterial(matname.c_str(), keyValues);
    printfdbg("Material %s created\n", matname.c_str());
    return newmat;
}


void colorWorld(bool on = 1) noexcept
{ 

    if (on) {
        if (!*g_Options.worldcoloractive) return;

        iff.g_pCVar->FindVar("r_drawspecificstaticprop")->SetValue(false);

        for (MaterialHandle_t h = iff.g_pMaterialSystem->FirstMaterial(); h != iff.g_pMaterialSystem->InvalidMaterial(); h = iff.g_pMaterialSystem->NextMaterial(h)) {
            IMaterial* pMaterial = iff.g_pMaterialSystem->GetMaterial(h);
            if (!pMaterial || !pMaterial->IsPrecached())
                continue;
            const char* textureGroup = pMaterial->GetTextureGroupName();
            if (strstr(textureGroup, "World textures"))
                pMaterial->ColorModulate(g_Options.worldcolor.value->r, g_Options.worldcolor.value->g, g_Options.worldcolor.value->b);
            if (strstr(textureGroup, "StaticProp"))
                pMaterial->ColorModulate(g_Options.staticpropcolor.value->r, g_Options.staticpropcolor.value->g, g_Options.staticpropcolor.value->b);
            if (strstr(textureGroup, "SkyBox"))
                pMaterial->ColorModulate(g_Options.staticpropcolor.value->r, g_Options.staticpropcolor.value->g, g_Options.staticpropcolor.value->b); 
            if (strstr(textureGroup, "Model"))
                pMaterial->ColorModulate(g_Options.modelcolor.value->r, g_Options.modelcolor.value->g, g_Options.modelcolor.value->b);
             
        }
          
    }
    else
    {
        iff.g_pCVar->FindVar("r_drawspecificstaticprop")->SetValue(true);

        for (MaterialHandle_t h = iff.g_pMaterialSystem->FirstMaterial(); h != iff.g_pMaterialSystem->InvalidMaterial(); h = iff.g_pMaterialSystem->NextMaterial(h)) {
            IMaterial* pMaterial = iff.g_pMaterialSystem->GetMaterial(h);
            if (!pMaterial || !pMaterial->IsPrecached())
                continue;
            const char* textureGroup = pMaterial->GetTextureGroupName();
            if (strstr(textureGroup, "World textures"))
                pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
            if (strstr(textureGroup, "StaticProp"))
                pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
            if (strstr(textureGroup, "Model"))
                pMaterial->ColorModulate(1.0f, 1.0f, 1.0f);
        }
         
    }
}

void ResetMisc()
{
    float f0 = 0, fm1 = -1.0f, f1 = 1.0f, f60 = 60.0f, f90 = 90.0f, f600 = 600.0f;
    SetValueUnrestricted("r_aspectratio", 0); memcpy(g_Options.aspectratio, &f0, 4);
    SetValueUnrestricted("mat_postprocess_enable", 1); memcpy(g_Options.postproc, &f0, 4);
    SetValueUnrestricted("cl_csm_enabled", 1); memcpy(g_Options.shadows, &f0, 4);
    SetValueUnrestricted("mat_fullbright", 0); memcpy(g_Options.fullbright, &f0, 4);
    SetValueUnrestricted("mat_drawgray", 0); memcpy(g_Options.drawgray, &f0, 4);
    SetValueUnrestricted("mat_showlowresimage", 0); memcpy(g_Options.showlowresimage, &f0, 4);
    SetValueUnrestricted("viewmodel_offset_x", 1.0f); memcpy(g_Options.viewmodel_x, &f1, 4);
    SetValueUnrestricted("viewmodel_offset_y", 1.0f); memcpy(g_Options.viewmodel_y, &f1, 4);
    SetValueUnrestricted("viewmodel_offset_z", -1.0f); memcpy(g_Options.viewmodel_z, &fm1, 4);
    memcpy(g_Options.viewmodel_ang_x, &f0, 4);
    memcpy(g_Options.viewmodel_ang_y, &f0, 4);
    memcpy(g_Options.viewmodel_ang_z, &f0, 4);
    memcpy(g_Options.fov, &f90, 4);
    SetValueUnrestricted("viewmodel_fov", 60.0f); memcpy(g_Options.viewmodel_fov, &f60, 4);
    memcpy(g_Options.viewmodel_moving, &f0, 4);
    SetValueUnrestricted("cl_viewmodel_shift_left_amt", GetVisibleFloat("cl_viewmodel_shift_left_amt"));
    SetValueUnrestricted("cl_viewmodel_shift_right_amt", GetVisibleFloat("cl_viewmodel_shift_right_amt"));
    SetValueUnrestricted("cl_bob_lower_amt", GetVisibleFloat("cl_bob_lower_amt"));
    SetValueUnrestricted("cl_bobamt_lat", GetVisibleFloat("cl_bobamt_lat"));
    SetValueUnrestricted("cl_bobamt_vert", GetVisibleFloat("cl_bobamt_vert"));
    SetValueUnrestricted("cl_wpn_sway_scale", GetVisibleFloat("cl_wpn_sway_scale"));
    iff.g_pCVar->FindVar("cl_ragdoll_gravity")->SetValue(600.0f);  memcpy(g_Options.ragdollgravity, &f600, 4);
    SetValueUnrestricted("cl_phys_timescale", 1.0f); memcpy(g_Options.ragdolltime, &f1, 4);
}

void RefreshThread(int* skinid)
{
    int localskinid = *skinid;
    *skinid = 0;
    iff.g_pClientState->ForceFullUpdate();
    Sleep(350);

    *skinid = localskinid;
    iff.g_pClientState->ForceFullUpdate();
    opt.needupdate = 1;
    ProtoFeatures.SendClientHello();
    ProtoFeatures.SendMatchmakingClient2GCHello();
}

struct float3 colorConverter(int hexValue)
{
    float3 rgbColor;
    rgbColor.b = ((hexValue >> 16) & 0xFF) / 255.0;
    rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;
    rgbColor.r = ((hexValue) & 0xFF) / 255.0;
    return rgbColor;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
static HWND window = NULL;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

#include "imgui/imgui_internal.h"

ImFont* ifont; ImFont* ifontBD; ImFont* ifontmini;
void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    auto a = ImFontConfig();
    a.FontBuilderFlags = 1 << 7 | 1 << 4 | 1 << 5;
    ifontBD = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeuib.ttf", 14);
    ifont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 14);
    ifontmini = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12);
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowMenuButtonPosition = 2;
    style->WindowTitleAlign = ImVec2(0.00f, 1.2f);
    style->WindowRounding = 6;
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.463f, 0.463f, 0.463f, 1.0f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->ButtonTextAlign = ImVec2(0.1f, 0.5f);
    style->TabRounding = 0;
    style->TabBorderSize = 1;
    style->Colors[ImGuiCol_Tab] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->WindowPadding = ImVec2(20.0f, 20.0f);
    style->ItemSpacing = ImVec2(7.0f, 15.0f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.757f, 0.757f, 0.757f, 1.0f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.757f, 0.757f, 0.757f, 1.0f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.757f, 0.757f, 0.757f, 1.0f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.612f, 0, 1.0f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.612f, 0, 1.0f);
    style->Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.612f, 0, 1.0f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.424f, 0.424f, 0.424f, 1.0f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.424f, 0.424f, 0.424f, 1.0f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.635f, 0.635f, 0.635f, 1.0f);
    style->ScrollbarRounding = 0;
    style->SelectableTextAlign = ImVec2(0, 0);

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(pDevice);

    printfdbg("Imgui initiated\n");
}
bool init = false;

 

ID3DXFont* font;
VMTHook* D3DHook = nullptr;


void DisableElements(bool check, bool dis)
{
    if (!check) {
        if (dis)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if (!dis)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
    }
}

long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
{
    static auto oEndScene = D3DHook->GetOriginal<long(__stdcall*)(IDirect3DDevice9*)>(42);


    if (!init)
    {
        D3DDEVICE_CREATION_PARAMETERS params;
        pDevice->GetCreationParameters(&params);
        window = params.hFocusWindow;
        oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
#ifdef DEBUG
        cout << "oWndProc " << oWndProc << endl;
#endif
        InitImGui(pDevice);
        init = true;
    }


    if (opt.show) {

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGuiStyle* style = &ImGui::GetStyle();

        ImGui::PushFont(ifontBD);

        ImGui::SetNextWindowSize(ImVec2{ 650, 455 });

        ImGui::Begin(XorStr("Spy's Seaside"), 0, ImGuiWindowFlags_NoResize);


        ImGui::PushFont(ifont);
        ImGui::BeginTabBar("1");

        if (ImGui::BeginTabItem("Effects"))
        {

            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);

                ImGui::Columns(2, nullptr, false);

                ImGui::Checkbox("Bullet trace beam", g_Options.beamtrace);

                DisableElements(*g_Options.beamtrace, 1);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);
                if (ImGui::BeginTable("ssplit", 2, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextRow(); ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::TextInputComboBox("Path to .vmt", g_Options.beampath.value->mystring, 255, opt.spriteslist, 0);
                    ImGui::InputInt("Beam type", g_Options.beamtype, 0, 0);
                    ImGui::InputFloat("Width", g_Options.beamwidth, 0, 0);
                    ImGui::InputFloat("Amplitude", g_Options.beamamplitude, 0, 0);
                    ImGui::InputFloat("Halo scale", g_Options.beamhaloscale, 0, 0);
                    ImGui::InputFloat("Start frame", g_Options.beamstartframe, 0, 0);
                    ImGui::InputInt("Segments", g_Options.beamsegments, 0, 0);

                    ImGui::TableNextColumn();

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f);
                    ImGui::ColorEdit4("##BeamColor", (float*)g_Options.beamcolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar);
                    ImGui::SameLine();
                    ImGui::Text("Color/Alpha");
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                    ImGui::InputFloat("Life##beam", g_Options.beamlife, 0, 0);
                    ImGui::InputFloat("End width", g_Options.beamendwidth, 0, 0);
                    ImGui::InputFloat("Speed##beam", g_Options.beamspeed, 0, 0);
                    ImGui::InputFloat("Fade length", g_Options.beamfadelength, 0, 0);
                    ImGui::InputFloat("Framerate", g_Options.beamframerate, 0, 0);
                    ImGui::InputInt("Flags", g_Options.beamflags, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);

                    ImGui::EndTable();
                }

                DisableElements(*g_Options.beamtrace, 0);
                 
                ImGui::NextColumn();

                ImGui::Checkbox("Bullet impact effect", g_Options.attacheffects);

                DisableElements(g_Options.attacheffects, 1);

                if (ImGui::BeginTable("split", 2, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextRow(); ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Interface", opt.EffTypes.at(g_Options.effects.value->type).c_str()))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < opt.EffTypes.size(); n++)
                        {
                            bool selected = (g_Options.effects.value->type == n);
                            if (ImGui::Selectable(opt.EffTypes.at(n).c_str(), selected, 0, ImVec2(0, 0), false))
                            {
                                g_Options.effects.value->type = n;
                                g_Options.effects.value->selectedc = 0;
                            }
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }

                    if (ImGui::BeginCombo("Name", opt.Map.at(opt.EffTypes.at(g_Options.effects.value->type)).at(g_Options.effects.value->selectedc).c_str()))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < opt.Map.at(opt.EffTypes.at(g_Options.effects.value->type)).size(); n++)
                        {
                            bool selected = (g_Options.effects.value->selectedc == n);
                            if (ImGui::Selectable(opt.Map.at(opt.EffTypes.at(g_Options.effects.value->type)).at(n).c_str(), selected, 0, ImVec2(0, 0), false))
                                g_Options.effects.value->selectedc = n;
                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);


                    ImGui::TableNextColumn();


                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 25.0f, ImGui::GetCursorPosY() - 20.0f));
                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);
                    if (ImGui::BeginChild("ChildR", ImVec2(135, 100), true, window_flags | ImGuiWindowFlags_AlwaysUseWindowPadding)) {

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                        switch (g_Options.effects.value->type)
                        {
                        case (0):
                        {
                            switch (g_Options.effects.value->selectedc)
                            {
                            case 0:
                            case 2:
                            {
                                ImGui::InputFloat("Size", &g_Options.effects.value->fx_fsize, 0, 0);
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                                ImGui::InputFloat("Speed", &g_Options.effects.value->fx_fspeed, 0, 0);
                                break;
                            }
                            case 1:
                            {
                                ImGui::InputInt("Magnitude", &g_Options.effects.value->fx_magnitude, 0, 0);
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                                ImGui::InputInt("Trail length", &g_Options.effects.value->fx_traillength, 0, 0);
                                break;
                            }
                            case 3:
                            {
                                ImGui::InputFloat("Scale", &g_Options.effects.value->fx_fsize, 0, 0);
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                                ImGui::InputInt("Type", &g_Options.effects.value->fx_itype, 0, 0);
                                break;
                            }
                            case 4:
                            case 6:
                            {
                                break;
                            }
                            case 5:
                            {
                                ImGui::Checkbox("Explosive", &g_Options.effects.value->fx_bexplosive);
                                break;
                            }

                            }
                            break;
                        }

                        case (1):
                        {
                            ImGui::InputInt("Colors", &g_Options.effects.value->icolors, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                            ImGui::InputFloat("Magnitude", &g_Options.effects.value->fmagtinude, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                            ImGui::InputFloat("Scale##de", &g_Options.effects.value->fscale, 0, 0);

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                            ImGui::InputInt("Flags##de", &g_Options.effects.value->fflags, 0, 0, ImGuiInputTextFlags_CharsHexadecimal); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                            ImGui::InputFloat("Radius", &g_Options.effects.value->fradius, 0, 0);

                            break;
                        }

                        case (2):
                        {
                            switch (g_Options.effects.value->selectedc)
                            {
                            case (0):
                            {
                                ImGui::InputFloat("Beam width", &g_Options.effects.value->fbeamwidth, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputFloat("Time visible", &g_Options.effects.value->fltimevisible, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);
                                ImGui::ColorEdit3("##teslacolor", (float*)g_Options.effects.value->teslacolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf);
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                                ImGui::InputFloat("Radius##tesla", &g_Options.effects.value->teslaradius, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputInt("Beams", &g_Options.effects.value->ibeams, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputText("Sprite name", g_Options.effects.value->szSpriteName, 256);
                                break;
                            }
                            case (1):
                            {
                                ImGui::InputFloat("Die", &g_Options.effects.value->fdie, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputInt("Style", &g_Options.effects.value->style, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);
                                ImGui::ColorEdit3("##dlightcolor", (float*)g_Options.effects.value->dlightcolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf);
                                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                                ImGui::InputFloat("Radius##dlight", &g_Options.effects.value->frad, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputInt("Key", &g_Options.effects.value->key, 0, 0); ImGui::InvisibleButton("##ib", ImVec2(0, 10.0f));
                                ImGui::InputFloat("Decay", &g_Options.effects.value->decay, 0, 0);

                                break;
                            }

                            }

                            break;
                        }

                        }


                        ImGui::EndChild();
                    }
                    DisableElements(g_Options.attacheffects, 0);

                    ImGui::EndTable();
                }




                if (ImGui::BeginTable("sssplit", 2, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextRow(); ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::Checkbox("Hit sound", g_Options.hitsound);
                    DisableElements(*g_Options.hitsound, 1);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7.0f);
                    ImGui::TextInputComboBox("Head shot", g_Options.hspath.value->mystring, 255, opt.soundslist, 0);
                    DisableElements(*g_Options.hitsound, 0);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 13.0f);
                    ImGui::Checkbox("Flashlight (L)", g_Options.flashlightON);
                    DisableElements(*g_Options.flashlightON, 1);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);
                    ImGui::TextInputComboBox("Path to .vmt ", g_Options.flashlightTexture.value->mystring, 255, opt.flashlightlist, 0);
                    DisableElements(*g_Options.flashlightON, 0);
                    ImGui::Checkbox("Nightvision (N)", g_Options.nvgsON);

                    ImGui::TableNextColumn();

                    ImGui::Checkbox("Hit marker", g_Options.hitmarker);
                    DisableElements(*g_Options.hitsound, 1);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);
                    ImGui::TextInputComboBox("Other bone", g_Options.obpath.value->mystring, 255, opt.soundslist, 0);
                    DisableElements(*g_Options.hitsound, 0);
                    DisableElements(*g_Options.flashlightON, 1);
                    ImGui::Checkbox("Cast shadows", g_Options.flashlightShadows);
                    ImGui::InputFloat("FOV", g_Options.flashlightFOV, 0, 0);
                    DisableElements(*g_Options.flashlightON, 0);

                    ImGui::EndTable();
                }
                 
                ImGui::Columns(1, nullptr, false);

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("World"))
        {

            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);


                ImGui::Columns(2, nullptr, false);


                if (ImGui::Checkbox("Skybox", g_Options.skyboxactive))
                {
                    if (g_Options.skyboxactive)
                    {
                        iff.g_pCVar->FindVar("sv_skyname")->SetValue((char*)g_Options.skyboxtemp.value.get());
                        iff.g_pCVar->FindVar("r_3dsky")->SetValue(false);
                    }
                    else iff.g_pCVar->FindVar("r_3dsky")->SetValue(true);
                }

                DisableElements(*g_Options.skyboxactive, 1);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);
                ImGui::PushItemWidth(170.0f);
                ImGui::TextInputComboBox("##W1", (char*)g_Options.skyboxtemp.value.get(), 255, opt.skyboxitems, 0);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Apply##W1", ImVec2(70, 22)))
                {
                    iff.g_pCVar->FindVar("sv_skyname")->SetValue((char*)g_Options.skyboxtemp.value.get());
                }

                DisableElements(*g_Options.skyboxactive, 0);

                if (ImGui::Checkbox("Weather", g_Options.weatheractive))
                {
                    if (!*g_Options.weatheractive) iff.g_pClientState->ForceFullUpdate();

                    iff.g_pCVar->FindVar("r_rainlength")->SetValue(*g_Options.rainlength);
                    iff.g_pCVar->FindVar("r_rainspeed")->SetValue(*g_Options.rainspeed);
                    iff.g_pCVar->FindVar("r_rainradius")->SetValue(*g_Options.rainradius);
                    iff.g_pCVar->FindVar("cl_windspeed")->SetValue(*g_Options.windspeed);
                    iff.g_pCVar->FindVar("r_rainwidth")->SetValue(*g_Options.rainwidth);
                    iff.g_pCVar->FindVar("r_RainSideVel")->SetValue(*g_Options.rainsidevel);
                    iff.g_pCVar->FindVar("r_rainalpha")->SetValue(*g_Options.rainalpha);
                }

                DisableElements(*g_Options.weatheractive, 1);

                if (ImGui::BeginTable("##Weather", 2, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextRow(); ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::InputInt("Precipitation type", g_Options.weathertype, 0, 0);

                    if (ImGui::InputFloat("Length", g_Options.rainlength, 0, 0))
                        iff.g_pCVar->FindVar("r_rainlength")->SetValue(*g_Options.rainlength);
                    if (ImGui::InputFloat("Speed", g_Options.rainspeed, 0, 0))
                        iff.g_pCVar->FindVar("r_rainspeed")->SetValue(*g_Options.rainspeed);
                    if (ImGui::InputFloat("Radius", g_Options.rainradius, 0, 0))
                        iff.g_pCVar->FindVar("r_rainradius")->SetValue(*g_Options.rainradius);

                    ImGui::TableNextColumn();

                    if (ImGui::InputFloat("Wind speed", g_Options.windspeed, 0, 0))
                        iff.g_pCVar->FindVar("cl_windspeed")->SetValue(*g_Options.windspeed);
                    if (ImGui::InputFloat("Width", g_Options.rainwidth, 0, 0))
                        iff.g_pCVar->FindVar("r_rainwidth")->SetValue(*g_Options.rainwidth);
                    if (ImGui::InputFloat("Side velocity", g_Options.rainsidevel, 0, 0))
                        iff.g_pCVar->FindVar("r_RainSideVel")->SetValue(*g_Options.rainsidevel);
                    if (ImGui::InputFloat("Alpha", g_Options.rainalpha, 0, 0))
                        iff.g_pCVar->FindVar("r_rainalpha")->SetValue(*g_Options.rainalpha);

                    ImGui::EndTable();
                }

                DisableElements(*g_Options.weatheractive, 0);

                ImGui::NextColumn();




                if (ImGui::BeginTable("##Fog", 4, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextColumn();

                    if (ImGui::Checkbox("Fog", g_Options.fogactive))
                    {
                        if (g_Options.fogactive)
                        {
                            SetValueUnrestricted("fog_enable", 1); 
                            SetValueUnrestricted("fog_enableskybox", 1);  
                            SetValueUnrestricted("fog_override", 1);
                            iff.g_pCVar->FindVar("fog_color")->SetValue(
                                std::string("").
                                append(to_string(g_Options.fogcolor.value->r * 255)).
                                append(" ").
                                append(to_string(g_Options.fogcolor.value->g * 255)).
                                append(" ").
                                append(to_string(g_Options.fogcolor.value->b * 255)).
                                append(" ").c_str()
                            );
                            iff.g_pCVar->FindVar("fog_colorskybox")->SetValue(
                                std::string("").
                                append(std::to_string(g_Options.fogcolor.value->r * 255)).
                                append(" ").
                                append(std::to_string(g_Options.fogcolor.value->g * 255)).
                                append(" ").
                                append(std::to_string(g_Options.fogcolor.value->b * 255)).
                                append(" ").c_str()
                            );
                            SetValueUnrestricted("fog_maxdensity", g_Options.fogdensity); 
                            SetValueUnrestricted("fog_maxdensityskybox", g_Options.fogdensity); 
                            SetValueUnrestricted("fog_start", g_Options.fogstart);
                            SetValueUnrestricted("fog_startskybox", g_Options.fogstart);
                            SetValueUnrestricted("fog_end", g_Options.fogend);
                            SetValueUnrestricted("fog_endskybox", g_Options.fogend); 
                        } 
                        else
                            SetValueUnrestricted("fog_override", 0);
                    }

                    ImGui::TableNextColumn();

                    DisableElements(*g_Options.fogactive, 1);

                    if (ImGui::ColorEdit3("##MyColor##W1", (float*)g_Options.fogcolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf))
                    {
                        iff.g_pCVar->FindVar("fog_color")->SetValue(
                            std::string("").
                            append(std::to_string(g_Options.fogcolor.value->r * 255)).
                            append(" ").
                            append(std::to_string(g_Options.fogcolor.value->g * 255)).
                            append(" ").
                            append(std::to_string(g_Options.fogcolor.value->b * 255)).
                            append(" ").c_str()
                        );

                        iff.g_pCVar->FindVar("fog_colorskybox")->SetValue(
                            std::string("").
                            append(std::to_string(g_Options.fogcolor.value->r * 255)).
                            append(" ").
                            append(std::to_string(g_Options.fogcolor.value->g * 255)).
                            append(" ").
                            append(std::to_string(g_Options.fogcolor.value->b * 255)).
                            append(" ").c_str()
                        );

                    }

                    ImGui::EndTable();
                }


                ImGui::PushItemWidth(252.0f);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);

                if (ImGui::SliderFloat("Max density", g_Options.fogdensity, 0.0f, 1.0f))
                {
                    iff.g_pCVar->FindVar("fog_maxdensity")->SetValue(g_Options.fogdensity);
                    iff.g_pCVar->FindVar("fog_maxdensityskybox")->SetValue(g_Options.fogdensity);
                }

                if (ImGui::SliderFloat("Start", g_Options.fogstart, 0.0f, 5000.0f))
                {
                    iff.g_pCVar->FindVar("fog_start")->SetValue(g_Options.fogstart);
                    iff.g_pCVar->FindVar("fog_startskybox")->SetValue(g_Options.fogstart);
                }
                if (ImGui::SliderFloat("End", g_Options.fogend, 0.0f, 5000.0f))
                {
                    iff.g_pCVar->FindVar("fog_end")->SetValue(g_Options.fogend);
                    iff.g_pCVar->FindVar("fog_endskybox")->SetValue(g_Options.fogend);
                }

                DisableElements(*g_Options.fogactive, 0);

                ImGui::InvisibleButton("##W4", ImVec2(0, 0.0f));

                if (ImGui::SliderFloat("Tonemap scale", g_Options.tonemapscale, 0.001f, 1.0f)) 
                    SetValueUnrestricted("mat_force_tonemap_scale", g_Options.tonemapscale);
                ImGui::PopItemWidth();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
                if (ImGui::Checkbox("Colorize map textures         ", g_Options.worldcoloractive))
                {
                    if (*g_Options.worldcoloractive) colorWorld();
                    else colorWorld(0);
                }

                DisableElements(*g_Options.worldcoloractive, 1);

                ImGui::SameLine();
                if (ImGui::Button("Modulate", ImVec2(70, 22)))
                    colorWorld();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);
                if (ImGui::BeginTable("##Colorworld", 4, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextColumn();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
                    ImGui::Text("World");
                    ImGui::ColorEdit3("##W1", (float*)g_Options.worldcolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf);

                    ImGui::TableNextColumn();
                    ImGui::Text("Prop/Sky");
                    ImGui::ColorEdit3("##W2", (float*)g_Options.staticpropcolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf);

                    ImGui::TableNextColumn();
                    ImGui::Text("Particles");
                    ImGui::ColorEdit4("##W4", (float*)g_Options.smokeskycolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar);


                    ImGui::TableNextColumn();
                    ImGui::Text("Model");
                    ImGui::ColorEdit3("##W3", (float*)g_Options.modelcolor.value.get(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf);

                    ImGui::EndTable();
                }

                DisableElements(*g_Options.worldcoloractive, 0);




                ImGui::Columns(1, nullptr, false);

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Models"))
        {
            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);

                static int selectedwep = 0;

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                if (ImGui::BeginTable("##Models", 2, ImGuiTableFlags_NoSavedSettings))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();


                    ImGui::PushItemWidth(170.0f);
                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Item", g_Options.models.value->arr[selectedwep].name))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < g_Options.models.value->itemcount; n++)
                        {
                            g_Options.models.value->arr[n].isSelected = (selectedwep == n);
                            if (ImGui::Selectable(g_Options.models.value->arr[n].name, g_Options.models.value->arr[n].isSelected, 0, ImVec2(0, 0), false))
                                selectedwep = n;
                            if (g_Options.models.value->arr[n].isSelected)
                                ImGui::SetItemDefaultFocus();
                        }

                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);
                    ImGui::PopItemWidth();

                    ImGui::SameLine();

                    if (ImGui::Button("Update", ImVec2(70, 22)))
                        iff.g_pClientState->ForceFullUpdate();

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.0f);

                    if (ImGui::Checkbox("Set viewmodel     ", &g_Options.models.value->arr[selectedwep].active))
                    {
                        if (!g_Options.models.value->arr[selectedwep].active) {
                            g_Options.models.value->arr[selectedwep].vmodel_hash = fnv2::hash(g_Options.models.value->arr[selectedwep].vmodel_orig);
                        }

                        if (g_Options.models.value->arr[selectedwep].active) {
                            g_Options.models.value->arr[selectedwep].vmodel_hash = fnv2::hash(g_Options.models.value->arr[selectedwep].vmodel_repl);
                        }

                        if (selectedwep < 2)
                        {

                            if (g_Options.models.value->arr[selectedwep].active)
                                *g_Options.playerloop_count += 1;
                            else *g_Options.playerloop_count -= 1;

                            printfdbg("pl count %d\n", *g_Options.playerloop_count);
                        }


                    }
                    ImGui::SameLine();
                    DisableElements(selectedwep > 2 ? true : false, 1);
                    ImGui::Checkbox("Via FindMDL hook", &g_Options.models.value->arr[selectedwep].findMDLmethode);
                    DisableElements(selectedwep > 2 ? true : false, 0);
                    ImGui::PushItemWidth(170.0f);
                    ImGui::InputText("Path to .mdl##viewmodel", g_Options.models.value->arr[selectedwep].vmodel_repl_temp, 256);
                    ImGui::PopItemWidth();

                    ImGui::SameLine();
                    if (ImGui::Button("Apply##viewmodel", ImVec2(70, 22)))
                    {
                        if (const auto modelprecache = iff.g_pNetworkStringTableContainer->FindTable("modelprecache"))
                            modelprecache->AddString(false, g_Options.models.value->arr[selectedwep].vmodel_repl_temp);

                        if (selectedwep > 2) {
                            for (int i = 0; i < 20; i++)
                                strcpy(g_Options.models.value->arr[selectedwep].seqs[i].seq_orig_def, "");

                            opt.needtogetseqact = selectedwep;
                        }

                        strcpy(g_Options.models.value->arr[selectedwep].vmodel_repl, g_Options.models.value->arr[selectedwep].vmodel_repl_temp);
                        g_Options.models.value->arr[selectedwep].vmodel_hash = fnv2::hashRuntime(g_Options.models.value->arr[selectedwep].vmodel_repl);
                    }


                    ImGui::TableNextColumn();

                    DisableElements(selectedwep == 2 ? false : true, 1);

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);

                    if (ImGui::Checkbox("Set scale", &g_Options.models.value->arr[selectedwep].active_scale))
                    {
                        if (g_Options.models.value->arr[selectedwep].active_scale)
                            *g_Options.entityloop_count += 1;
                        else *g_Options.entityloop_count -= 1;

                        printfdbg("el count %d\n", *g_Options.entityloop_count);
                    }

                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 30.0f);

                    DisableElements(g_Options.models.value->arr[selectedwep].active_scale, 1);
                    ImGui::PushItemWidth(252.0f);
                    ImGui::SliderFloat("##scale", &g_Options.models.value->arr[selectedwep].scale, 0.0f, 10.0f);
                    ImGui::PopItemWidth();
                    DisableElements(g_Options.models.value->arr[selectedwep].active_scale, 0);

                    DisableElements(selectedwep == 2 ? false : true, 0);

                    DisableElements(selectedwep > 2 ? true : false, 1);

                    if (ImGui::Checkbox("Set worldmodel", &g_Options.models.value->arr[selectedwep].active_w))
                    {

                        if (g_Options.models.value->arr[selectedwep].active_w)
                            *g_Options.entityloop_count += 1;
                        else *g_Options.entityloop_count -= 1;

                        printfdbg("el count %d\n", *g_Options.entityloop_count);

                    }

                    ImGui::PushItemWidth(170.0f);
                    ImGui::InputText("Path to .mdl##worldmodel", g_Options.models.value->arr[selectedwep].wmodel_repl_temp, 256);
                    ImGui::PopItemWidth();

                    ImGui::SameLine();
                    if (ImGui::Button("Apply##worldmodel", ImVec2(70, 22)))
                    {
                        strcpy(g_Options.models.value->arr[selectedwep].wmodel_repl, g_Options.models.value->arr[selectedwep].wmodel_repl_temp);
                        g_Options.models.value->arr[selectedwep].wmodel_hash = fnv2::hashRuntime(g_Options.models.value->arr[selectedwep].wmodel_repl);
                    }

                    DisableElements(selectedwep > 2 ? true : false, 0);



                    ImGui::EndTable();
                }



                ImGui::Columns(2, nullptr, false);

                DisableElements(selectedwep > 2 ? true : false, 1);
                if (ImGui::Checkbox("Set animations", &g_Options.models.value->arr[selectedwep].seq_active))
                    opt.needtogetseqact = selectedwep;
                DisableElements(selectedwep > 2 ? true : false, 0);


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);

                DisableElements(g_Options.models.value->arr[selectedwep].seq_active, 1);

                static char ccc[20][5] = { "##0","##1","##2","##3","##4","##5","##6","##7","##8","##9","##10","##11","##12","##13","##14","##15","##16","##17","##18","##19" };

                static ImGuiTableFlags flags = ImGuiTableFlags_None;

                static ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                if (ImGui::BeginChild("ChildR", ImVec2(252, 150), true, window_flags | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY() - 10.0f));
                    if (ImGui::BeginTable("split", 2, flags | ImGuiTableFlags_NoSavedSettings))
                    {
                        for (int i = 0; i < 20; i++)
                        {
                            char buf[32];
                            sprintf(buf, "%01d:", i);
                            ImGui::TableNextColumn();
                            ImGui::Text(buf);
                            ImGui::SameLine();
                            ImGui::Text(g_Options.models.value->arr[selectedwep].seqs[i].seq_orig_def);

                            ImGui::TableNextColumn();

                            ImGui::PushItemWidth(100.0f);

                            style->WindowPadding = ImVec2(5.0f, 5.0f);
                            if (ImGui::BeginCombo(ccc[i], g_Options.models.value->arr[selectedwep].seqs[g_Options.models.value->arr[selectedwep].seqs[i].seq_repl].seq_repl_def_preview))
                            {
                                ImGui::PushFont(ifontmini);
                                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                                for (int n = 0; n < 20; n++)
                                {
                                    g_Options.models.value->arr[selectedwep].seqs[i].isSelected[n] = (g_Options.models.value->arr[selectedwep].seqs[i].seq_repl == n);
                                    if (ImGui::Selectable(g_Options.models.value->arr[selectedwep].seqs[n].seq_repl_def_preview, g_Options.models.value->arr[selectedwep].seqs[i].isSelected[n], 0, ImVec2(0, 0), false))
                                        g_Options.models.value->arr[selectedwep].seqs[i].seq_repl = n;
                                    if (g_Options.models.value->arr[selectedwep].seqs[i].isSelected[n])
                                        ImGui::SetItemDefaultFocus();
                                }

                                style->ItemSpacing = ImVec2(7.0f, 15.0f);
                                ImGui::PushFont(ifont);
                                ImGui::EndCombo();
                            }
                            style->WindowPadding = ImVec2(20.f, 20.0f);

                            ImGui::PopItemWidth();

                        }
                        ImGui::EndTable();
                    }

                    ImGui::EndChild();
                }

                DisableElements(g_Options.models.value->arr[selectedwep].seq_active, 0);

                style->ItemSpacing = ImVec2(7.0f, 15.0f);


                ImGui::NextColumn();



                static const char sss[20][6] = { "##0s","##1s","##2s","##3s","##4s","##5s","##6s","##7s","##8s","##9s","##10s",
                    "##11s","##12s","##13s","##14s","##15s","##16s","##17s","##18s","##19s" };

                static const char sss1[5][6] = { "##0v","##1v","##2v","##3v","##4v" };
                static const char sss2[5][6] = { "##0p","##1p","##2p","##3p","##4p" };

                DisableElements(selectedwep > 3 ? true : false, 1);
                if (ImGui::Checkbox("Set sounds", &g_Options.models.value->arr[selectedwep].sound_active))
                {
                    if (g_Options.models.value->arr[selectedwep].sound_active)
                        *g_Options.soundhook_count += 1;
                    else *g_Options.soundhook_count -= 1;

                    printfdbg("sh count %d\n", *g_Options.soundhook_count);

                }
                DisableElements(selectedwep > 3 ? true : false, 0);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);

                DisableElements(g_Options.models.value->arr[selectedwep].sound_active, 1);



                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                if (ImGui::BeginChild("ChildS", ImVec2(252, 150), true, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar)) {
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 10.0f, ImGui::GetCursorPosY() - 10.0f));
                    if (ImGui::BeginTable("splits", 2, ImGuiTableFlags_NoSavedSettings))
                    {
                        for (int i = 0; i < g_Options.models.value->arr[selectedwep].soundcount; i++)
                        {

                            ImGui::TableNextColumn();


                            ImGui::Text(g_Options.models.value->arr[selectedwep].sounds[i].name);

                            ImGui::InvisibleButton("##xz", ImVec2(0, 14.0f));
                            ImGui::Text("Pitch");
                            ImGui::SameLine();
                            ImGui::InputInt((char*)sss2[i], &g_Options.models.value->arr[selectedwep].sounds[i].pitch, 0, 0);


                            ImGui::TableNextColumn();

                            ImGui::PushItemWidth(106.0f);
                            ImGui::InputText((char*)sss[i], g_Options.models.value->arr[selectedwep].sounds[i].wavpath, 255);
                            ImGui::PopItemWidth();

                            ImGui::Text("Volume");
                            ImGui::SameLine();

                            ImGui::PushItemWidth(56.0f);
                            ImGui::InputFloat((char*)sss1[i], &g_Options.models.value->arr[selectedwep].sounds[i].volume, 0, 0);


                            ImGui::PopItemWidth();

                        }
                        ImGui::EndTable();
                    }

                    ImGui::EndChild();
                }

                style->ItemSpacing = ImVec2(7.0f, 15.0f);
                DisableElements(g_Options.models.value->arr[selectedwep].sound_active, 0);


                ImGui::Columns(1, nullptr, false);

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Materials"))
        {
            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);


                ImGui::Columns(2, nullptr, false);
                static int selectedwep = 0;


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                ImGui::PushItemWidth(170.0f);

                style->WindowPadding = ImVec2(5.0f, 5.0f);
                if (ImGui::BeginCombo("Item", g_Options.materials.value->arr[selectedwep].name))
                {
                    ImGui::PushFont(ifontmini);
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);
                    for (int n = 0; n < g_Options.materials.value->itemcount; n++)
                    {
                        g_Options.materials.value->arr[n].isSelected = (selectedwep == n);
                        if (ImGui::Selectable(g_Options.materials.value->arr[n].name, g_Options.materials.value->arr[n].isSelected, 0, ImVec2(0, 0), false))
                            selectedwep = n;
                        if (g_Options.materials.value->arr[n].isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    style->ItemSpacing = ImVec2(7.0f, 15.0f);
                    ImGui::PushFont(ifont);
                    ImGui::EndCombo();
                }
                style->WindowPadding = ImVec2(20.f, 20.0f);

                ImGui::PopItemWidth();

                ImGui::SameLine();

                if (ImGui::Checkbox("Active##S1", &g_Options.materials.value->arr[selectedwep].active))
                {
                    if (g_Options.materials.value->arr[selectedwep].active)
                        *g_Options.dmeloop_count += 1;
                    else *g_Options.dmeloop_count -= 1;

                    printfdbg("dl count %d\n", *g_Options.dmeloop_count);
                }

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);

                DisableElements(g_Options.materials.value->arr[selectedwep].active, 1);

                ImGui::PushItemWidth(170.0f);
                ImGui::TextInputComboBox("Texture", g_Options.materials.value->arr[selectedwep].texture_temp, 255, opt.dmeitems, 0);
                ImGui::PopItemWidth();

                ImGui::SameLine();

                if (ImGui::Button("Apply", ImVec2(70, 22)))
                {
                    if (strstr(g_Options.materials.value->arr[selectedwep].texture_temp, "(0)"))
                        g_Options.materials.value->arr[selectedwep].customtextureselected = 0;
                    else if (strstr(g_Options.materials.value->arr[selectedwep].texture_temp, "(1)"))
                        g_Options.materials.value->arr[selectedwep].customtextureselected = 1;
                    else if (strstr(g_Options.materials.value->arr[selectedwep].texture_temp, "(2)"))
                        g_Options.materials.value->arr[selectedwep].customtextureselected = 2;
                    else if (strstr(g_Options.materials.value->arr[selectedwep].texture_temp, "(3)"))
                        g_Options.materials.value->arr[selectedwep].customtextureselected = 3;
                    else if (strstr(g_Options.materials.value->arr[selectedwep].texture_temp, "(4)"))
                        g_Options.materials.value->arr[selectedwep].customtextureselected = 4;
                    else g_Options.materials.value->arr[selectedwep].customtextureselected = -1;

                    printfdbg("Custom texture selected? %d\n", g_Options.materials.value->arr[selectedwep].customtextureselected);
                    strcpy(g_Options.materials.value->arr[selectedwep].texture, g_Options.materials.value->arr[selectedwep].texture_temp);
                }


                ImGui::NextColumn();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

                if (ImGui::BeginTable("##tablexz", 2))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("Wireframe", &g_Options.materials.value->arr[selectedwep].wireframe);
                    ImGui::Checkbox("Flat", &g_Options.materials.value->arr[selectedwep].flat);
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("No draw", &g_Options.materials.value->arr[selectedwep].nodraw);
                    ImGui::ColorEdit4("MyColor##0", (float*)&g_Options.materials.value->arr[selectedwep].coloralpha, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar);
                    ImGui::SameLine();
                    ImGui::Text("Color/Alpha");

                    ImGui::EndTable();
                }

                DisableElements(g_Options.materials.value->arr[selectedwep].active, 0);

                ImGui::Columns(1, nullptr, false);
                ImGui::Separator();


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                static int item_current_idx = 0;

                ImGui::InputTextMultiline("KeyValue", g_Options.customtextures.value->arr[item_current_idx].keyvalue, 0x1000, ImVec2(467, 200), ImGuiInputTextFlags_AllowTabInput);

                ImGui::SameLine();

                ImVec2 savepos = ImGui::GetCursorPos();


                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                if (ImGui::BeginListBox("Custom textures", ImVec2(98, 100)))
                {
                    ImGui::PushFont(ifontmini);
                    for (int n = 0; n < 5; n++)
                    {
                        g_Options.customtextures.value->arr[n].IsSelected = (item_current_idx == n);
                        if (ImGui::Selectable(g_Options.customtextures.value->arr[n].Preview, g_Options.customtextures.value->arr[n].IsSelected, 0, ImVec2(0, 0), true))
                            item_current_idx = n;

                        if (g_Options.customtextures.value->arr[n].IsSelected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                    ImGui::PushFont(ifont);
                }
                style->ItemSpacing = ImVec2(7.0f, 15.0f);

                savepos.y += 130.0f;
                ImGui::SetCursorPos(savepos);

                ImGui::PushItemWidth(100.0f);
                ImGui::InputText("Name", g_Options.customtextures.value->arr[item_current_idx].Name, 256);
                ImGui::PopItemWidth();

                savepos.y += 40.0f;
                ImGui::SetCursorPos(savepos);

                if (ImGui::Button("Apply##refresh", ImVec2(70, 22)))
                {
                    if (g_Options.customtextures.value->arr[item_current_idx].Name[0] == 0x0 && g_Options.customtextures.value->arr[item_current_idx].keyvalue[0] == 0x0)
                    {
                        char newpreview[256] = "(";
                        strcat_s(newpreview, to_string(item_current_idx).c_str());
                        strcat_s(newpreview, ") null");
                        strcpy(g_Options.customtextures.value->arr[item_current_idx].Preview, newpreview);
                        g_Options.customtextures.value->arr[item_current_idx].texturelink = nullptr;
                    }
                    else
                    {
                        char newpreview[256] = "(";
                        strcat_s(newpreview, to_string(item_current_idx).c_str());
                        strcat_s(newpreview, ") ");
                        strcat_s(newpreview, g_Options.customtextures.value->arr[item_current_idx].Name);
                        strcpy(g_Options.customtextures.value->arr[item_current_idx].Preview, newpreview);

                        g_Options.customtextures.value->arr[item_current_idx].texturelink = CreateMaterial(
                            string(g_Options.customtextures.value->arr[item_current_idx].Name),
                            string(g_Options.customtextures.value->arr[item_current_idx].keyvalue));
                    }

                    opt.dmeitems.at(item_current_idx) = g_Options.customtextures.value->arr[item_current_idx].Preview;


                }

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }


        if (ImGui::BeginTabItem("Inventory"))
        {

            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);


                static int selectedwep = 0;
                ImGui::Text("Item");
                ImGui::SameLine();
                ImGui::PushItemWidth(390.0f);
                style->WindowPadding = ImVec2(5.0f, 5.0f);
                if (ImGui::BeginCombo("##label1", g_Options.weapons.value->arr[selectedwep].name))
                {
                    ImGui::PushFont(ifontmini);
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);
                    for (int n = 0; n < g_Options.weapons.value->weaponzcount; n++)
                    {
                        g_Options.weapons.value->arr[n].isSelected = (selectedwep == n);
                        if (ImGui::Selectable(g_Options.weapons.value->arr[n].name, g_Options.weapons.value->arr[n].isSelected, 0, ImVec2(0, 0), false))
                            selectedwep = n;
                        if (g_Options.weapons.value->arr[n].isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    style->ItemSpacing = ImVec2(7.0f, 15.0f);
                    ImGui::PushFont(ifont);
                    ImGui::EndCombo();
                }
                style->WindowPadding = ImVec2(20.f, 20.0f);
                ImGui::SameLine();

                ImGui::Checkbox("Active##S1", &g_Options.weapons.value->arr[selectedwep].active);

                ImGui::SameLine();
                if (ImGui::Button("Update", ImVec2(70, 22))) {
                    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)RefreshThread, &g_Options.weapons.value->arr[selectedwep].skinid, 0, 0);
                }

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);

                DisableElements(g_Options.weapons.value->arr[selectedwep].active, 1);

                ImGui::Columns(2, nullptr, false);

                style->WindowPadding = ImVec2(5.0f, 5.0f);
                ImGui::PushItemWidth(252.0f);
                if (ImGui::BeginCombo("Paint kit ID", opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].Preview.c_str()))
                {
                    ImGui::PushFont(ifontmini);
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);

                    for (int n = 0; n < opt.sc_skins.size(); n++)
                    {
                        opt.sc_skins[n].isSelected = (g_Options.weapons.value->arr[selectedwep].skinidc == n);
                        if (ImGui::Selectable(opt.sc_skins[n].Preview.c_str(), opt.sc_skins[n].isSelected, 0, ImVec2(0, 0), false))
                        {
                            g_Options.weapons.value->arr[selectedwep].skinidc = n;
                            g_Options.weapons.value->arr[selectedwep].skinid = opt.sc_skins[n].id;


                            g_Options.weapons.value->arr[selectedwep].rarity = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].rarity;
                            g_Options.weapons.value->arr[selectedwep].pearl = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].pearl;
                            g_Options.weapons.value->arr[selectedwep].color1 = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color1;
                            g_Options.weapons.value->arr[selectedwep].color2 = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color2;
                            g_Options.weapons.value->arr[selectedwep].color3 = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color3;
                            g_Options.weapons.value->arr[selectedwep].color4 = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color4;
                        }

                        if (opt.sc_skins[n].isSelected)
                            ImGui::SetItemDefaultFocus();

                    }

                    style->ItemSpacing = ImVec2(7.0f, 15.0f);
                    ImGui::PushFont(ifont);

                    ImGui::EndCombo();
                }
                style->WindowPadding = ImVec2(20.f, 20.0f);
                ImGui::PopItemWidth();



                if (ImGui::BeginTable("##table1", 2))
                {
                    ImGui::TableNextRow();

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    ImGui::InputInt("Seed", &g_Options.weapons.value->arr[selectedwep].seed, 0);
                    ImGui::InputInt("StatTrak(tm)", &g_Options.weapons.value->arr[selectedwep].stattrak, 0);
                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Quality", qualities[g_Options.weapons.value->arr[selectedwep].qualityc].name.c_str()))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < qualities.size(); n++)
                        {
                            qualities[n].isSelected = (g_Options.weapons.value->arr[selectedwep].quality == qualities[n].defindex);
                            if (ImGui::Selectable(qualities[n].name.c_str(), qualities[n].isSelected, 0, ImVec2(0, 0), false))
                            {
                                g_Options.weapons.value->arr[selectedwep].qualityc = n;
                                g_Options.weapons.value->arr[selectedwep].quality = qualities[n].defindex;
                            }

                            if (qualities[n].isSelected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);


                    ImGui::TableNextColumn();

                    float thisy = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(thisy - 20.0f);
                    ImGui::Text("Custom colors");
                    ImGui::SetCursorPosY(thisy);

                    float3 color1 = colorConverter(opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color1);
                    if (ImGui::ColorEdit3("##Custom colors", (float*)&color1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf))
                    {
                        int color1int = 0xFF000000 + ((int)(color1.b * 255) << 16) | ((int)(color1.g * 255) << 8) | (int)(color1.r * 255);
                        opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color1 = color1int;
                        CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                        pk->color1 = color1int;
                        g_Options.weapons.value->arr[selectedwep].color1 = color1int;
                    }
                    ImGui::SameLine();
                    float3 color2 = colorConverter(opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color2);
                    if (ImGui::ColorEdit3("##Color2", (float*)&color2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf))
                    {
                        int color2int = 0xFF000000 + ((int)(color2.b * 255) << 16) | ((int)(color2.g * 255) << 8) | (int)(color2.r * 255);
                        opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color2 = color2int;
                        CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                        pk->color2 = color2int;
                        g_Options.weapons.value->arr[selectedwep].color2 = color2int;
                    }
                    ImGui::SameLine();
                    float3 color3 = colorConverter(opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color3);
                    if (ImGui::ColorEdit3("##Color3", (float*)&color3, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf))
                    {
                        int color3int = 0xFF000000 + ((int)(color3.b * 255) << 16) | ((int)(color3.g * 255) << 8) | (int)(color3.r * 255);
                        opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color3 = color3int;
                        CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                        pk->color3 = color3int;
                        g_Options.weapons.value->arr[selectedwep].color3 = color3int;
                    }
                    ImGui::SameLine();
                    float3 color4 = colorConverter(opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color4);
                    if (ImGui::ColorEdit3("##Color4", (float*)&color4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf))
                    {
                        int color4int = 0xFF000000 + ((int)(color4.b * 255) << 16) | ((int)(color4.g * 255) << 8) | (int)(color4.r * 255);
                        opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].color4 = color4int;
                        CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                        pk->color4 = color4int;
                        g_Options.weapons.value->arr[selectedwep].color4 = color4int;
                    }
                    ImGui::Checkbox("Freeze count", &g_Options.weapons.value->arr[selectedwep].ownerunk);
                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Rarity", rarities[opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].rarity].name.c_str()))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < rarities.size(); n++)
                        {
                            rarities[n].isSelected = (opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].rarity == rarities[n].defindex);
                            if (ImGui::Selectable(rarities[n].name.c_str(), rarities[n].isSelected, 0, ImVec2(0, 0), false))
                            {
                                opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].rarity = rarities[n].defindex;
                                CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                                pk->rarity = rarities[n].defindex;
                                g_Options.weapons.value->arr[selectedwep].rarity = rarities[n].defindex;
                            }

                            if (rarities[n].isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);



                    ImGui::EndTable();

                }


                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

                ImGui::PushItemWidth(252.0f);
                ImGui::SliderFloat("Wear", &g_Options.weapons.value->arr[selectedwep].wear, 0.0f, 1.0f);
                if (ImGui::SliderFloat("Pearlescent", &opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].pearl, 0.0f, 255.0f))
                {
                    CPaintKit* pk = (CPaintKit*)opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].link;
                    pk->pearlescent = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].pearl;
                    g_Options.weapons.value->arr[selectedwep].pearl = opt.sc_skins[g_Options.weapons.value->arr[selectedwep].skinidc].pearl;
                }
                ImGui::PopItemWidth();


                if (ImGui::BeginTable("##table3", 2))
                {
                    ImGui::TableNextRow();

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    style->WindowPadding = ImVec2(5.0f, 5.0f);

                    bool disabledDefIndex = (selectedwep > 5) && (selectedwep < (g_Options.weapons.value->weaponzcount - 1)) ? false : true;
                    bool disabledKFIcon = ((selectedwep > 0) && (selectedwep < 6)) || (selectedwep == g_Options.weapons.value->weaponzcount - 1) ? false : true;

                    DisableElements(disabledDefIndex, 1);


                    if (selectedwep == 0) {
                        if (ImGui::BeginCombo("Definition", knives[g_Options.weapons.value->arr[selectedwep].modelcount].Preview.c_str()))
                        {
                            ImGui::PushFont(ifontmini);
                            style->ItemSpacing = ImVec2(7.0f, 2.0f);

                            for (int n = 0; n < knives.size(); n++)
                            {
                                knives[n].isSelected = (g_Options.weapons.value->arr[selectedwep].modelcount == n);
                                if (ImGui::Selectable(knives[n].Preview.c_str(), knives[n].isSelected, 0, ImVec2(0, 0), false))
                                {
                                    g_Options.weapons.value->arr[selectedwep].modelcount = n;
                                    g_Options.weapons.value->arr[selectedwep].modeldefindex = knives[n].defindex;
                                    strcpy(g_Options.weapons.value->arr[selectedwep].model, knives[n].viewmodel.c_str());
                                    strcpy(g_Options.weapons.value->arr[selectedwep].worldmodel, knives[n].worldmodel.c_str());

                                }

                                if (knives[n].isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }

                            style->ItemSpacing = ImVec2(7.0f, 15.0f);
                            ImGui::PushFont(ifont);
                            ImGui::EndCombo();
                        }
                    }
                    else if (selectedwep == 1)
                    {
                        if (ImGui::BeginCombo("Definition", gloves[g_Options.weapons.value->arr[selectedwep].modelcount].Preview.c_str()))
                        {
                            ImGui::PushFont(ifontmini);
                            style->ItemSpacing = ImVec2(7.0f, 2.0f);
                            for (int n = 0; n < gloves.size(); n++)
                            {
                                gloves[n].isSelected = (g_Options.weapons.value->arr[selectedwep].modelcount == n);
                                if (ImGui::Selectable(gloves[n].Preview.c_str(), gloves[n].isSelected, 0, ImVec2(0, 0), false))
                                {
                                    g_Options.weapons.value->arr[selectedwep].modelcount = n;
                                    g_Options.weapons.value->arr[selectedwep].modeldefindex = gloves[n].defindex;
                                    strcpy(g_Options.weapons.value->arr[selectedwep].model, gloves[n].viewmodel.c_str());
                                    strcpy(g_Options.weapons.value->arr[selectedwep].worldmodel, gloves[n].worldmodel.c_str());
                                }
                                if (gloves[n].isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            style->ItemSpacing = ImVec2(7.0f, 15.0f);
                            ImGui::PushFont(ifont);
                            ImGui::EndCombo();
                        }
                    }
                    else if (selectedwep == 2 || selectedwep == 3)
                    {
                        if (ImGui::BeginCombo("Definition", agents[g_Options.weapons.value->arr[selectedwep].modelcount].Preview.c_str()))
                        {
                            ImGui::PushFont(ifontmini);
                            style->ItemSpacing = ImVec2(7.0f, 2.0f);
                            for (int n = 0; n < agents.size(); n++)
                            {
                                agents[n].isSelected = (g_Options.weapons.value->arr[selectedwep].modelcount == n);
                                if (ImGui::Selectable(agents[n].Preview.c_str(), agents[n].isSelected, 0, ImVec2(0, 0), false))
                                {
                                    g_Options.weapons.value->arr[selectedwep].modelcount = n;
                                    g_Options.weapons.value->arr[selectedwep].modeldefindex = agents[n].defindex;
                                    strcpy(g_Options.weapons.value->arr[selectedwep].model, agents[n].viewmodel.c_str());

                                }
                                if (agents[n].isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            style->ItemSpacing = ImVec2(7.0f, 15.0f);
                            ImGui::PushFont(ifont);
                            ImGui::EndCombo();
                        }
                    }

                    else if (selectedwep == 4)
                    {
                        if (ImGui::BeginCombo("Definition", medals[g_Options.weapons.value->arr[selectedwep].modelcount].Preview.c_str()))
                        {
                            ImGui::PushFont(ifontmini);
                            style->ItemSpacing = ImVec2(7.0f, 2.0f);
                            for (int n = 0; n < medals.size(); n++)
                            {
                                medals[n].isSelected = (g_Options.weapons.value->arr[selectedwep].modelcount == n);
                                if (ImGui::Selectable(medals[n].Preview.c_str(), medals[n].isSelected, 0, ImVec2(0, 0), false))
                                {
                                    g_Options.weapons.value->arr[selectedwep].modelcount = n;
                                    g_Options.weapons.value->arr[selectedwep].modeldefindex = medals[n].defindex;
                                }
                                if (medals[n].isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            style->ItemSpacing = ImVec2(7.0f, 15.0f);
                            ImGui::PushFont(ifont);
                            ImGui::EndCombo();
                        }
                    }
                    else if (selectedwep == 5)
                    {
                        if (ImGui::BeginCombo("Definition", musickits[g_Options.weapons.value->arr[selectedwep].modelcount].Preview.c_str()))
                        {
                            ImGui::PushFont(ifontmini);
                            style->ItemSpacing = ImVec2(7.0f, 2.0f);
                            for (int n = 0; n < musickits.size(); n++)
                            {
                                musickits[n].isSelected = (g_Options.weapons.value->arr[selectedwep].modelcount == n);
                                if (ImGui::Selectable(musickits[n].Preview.c_str(), musickits[n].isSelected, 0, ImVec2(0, 0), false))
                                {
                                    g_Options.weapons.value->arr[selectedwep].modelcount = n;
                                    g_Options.weapons.value->arr[selectedwep].modeldefindex = musickits[n].defindex;

                                }
                                if (musickits[n].isSelected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            style->ItemSpacing = ImVec2(7.0f, 15.0f);
                            ImGui::PushFont(ifont);
                            ImGui::EndCombo();
                        }
                    }
                    else if (selectedwep == (g_Options.weapons.value->weaponzcount - 1))
                    {
                        if (ImGui::InputInt("Definition", &g_Options.weapons.value->arr[selectedwep].defindex, 0, 0))
                            g_Options.weapons.value->arr[selectedwep].modeldefindex = g_Options.weapons.value->arr[selectedwep].defindex;
                    }
                    else
                    {
                        ImGui::BeginCombo("Definition", "Disabled");
                        ImGui::EndCombo;
                    }

                    style->WindowPadding = ImVec2(20.f, 20.0f);

                    DisableElements(disabledDefIndex, 0);
                      
                    ImGui::TableNextColumn();
                     
                    DisableElements(disabledKFIcon, 1);
                      
                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Killfeed icon", g_Options.weapons.value->arr[selectedwep].killfeediconreplace))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < opt.killfeedicons.size(); n++)
                        {
                            if (ImGui::Selectable(opt.killfeedicons[n].c_str(), false, 0, ImVec2(0, 0), false))
                            {
                                strcpy(g_Options.weapons.value->arr[selectedwep].killfeediconreplace, opt.killfeedicons[n].c_str());
                                if (n > 0)
                                    g_Options.weapons.value->arr[selectedwep].killfeediconactive = 1;
                                else
                                    g_Options.weapons.value->arr[selectedwep].killfeediconactive = 0;
                            }
                        }
                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);
                      
                    DisableElements(disabledKFIcon, 0);

                    ImGui::EndTable();
                }


                ImGui::NextColumn();


                static int stickerselected = 0;
                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                if (ImGui::BeginListBox("Stickers##lb", ImVec2(251.0f, 5 * ImGui::GetTextLineHeightWithSpacing())))
                {
                    ImGui::PushFont(ifontmini);
                    for (int n = 0; n < 5; n++)
                    {
                        g_Options.weapons.value->arr[selectedwep].stickers[n].isselected = (stickerselected == n);
                        if (ImGui::Selectable(g_Options.weapons.value->arr[selectedwep].stickers[n].name,
                            g_Options.weapons.value->arr[selectedwep].stickers[n].isselected, 0, ImVec2(0, 0), true)) {
                            stickerselected = n;
                        }

                        if (g_Options.weapons.value->arr[selectedwep].stickers[n].isselected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::PushFont(ifont);
                    ImGui::EndListBox();
                }
                style->ItemSpacing = ImVec2(7.0f, 15.0f);

                ImGui::InvisibleButton("##invis", ImVec2(0, 10.0f));

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

                style->WindowPadding = ImVec2(5.0f, 5.0f);
                ImGui::PushItemWidth(252.0f);

                if (ImGui::BeginCombo("Sticker ID", opt.sc_stickers[g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].stickerc].Preview.c_str()))
                {
                    ImGui::PushFont(ifontmini);
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);

                    for (int n = 0; n < opt.sc_stickers.size(); n++)
                    {
                        opt.sc_stickers[n].isSelected = (g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].stickerc == n);
                        if (ImGui::Selectable(opt.sc_stickers[n].Preview.c_str(), opt.sc_stickers[n].isSelected, 0, ImVec2(0, 0), false))
                        {
                            g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].stickerc = n;
                            g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].stickerid = opt.sc_stickers[n].id;

                            char newname[64] = "(";
                            strcat_s(newname, to_string(stickerselected).c_str());
                            strcat_s(newname, ") ");
                            strcat_s(newname, opt.sc_stickers[g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].stickerc].Name.c_str());

                            strcpy(g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].name,
                                newname);
                        }
                        if (opt.sc_stickers[n].isSelected)
                            ImGui::SetItemDefaultFocus();

                    }
                    style->ItemSpacing = ImVec2(7.0f, 15.0f);
                    ImGui::PushFont(ifont);

                    ImGui::EndCombo();
                }
                style->WindowPadding = ImVec2(20.f, 20.0f);
                ImGui::PopItemWidth();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7.0f);


                ImGui::PushItemWidth(252.0f);
                ImGui::SliderFloat("Wear##st", &g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].wear, 0, 1.0f);
                ImGui::SliderFloat("Scale##st", &g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].scale, 2.0f, 0.0f);
                ImGui::SliderFloat("Rotation##st", &g_Options.weapons.value->arr[selectedwep].stickers[stickerselected].rotation, 0, 360.0f);
                ImGui::PopItemWidth();

                if (ImGui::BeginTable("##table5", 2))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::InputText("Name tag", g_Options.weapons.value->arr[selectedwep].nametag, 32);
                    ImGui::TableNextColumn();
                    ImGui::InputText("Desc tag", g_Options.weapons.value->arr[selectedwep].desctag, 32);

                    ImGui::EndTable();
                }

                ImGui::Columns(1, nullptr, false);

                DisableElements(g_Options.weapons.value->arr[selectedwep].active, 0);

                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Profile"))
        {
            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);


                if (ImGui::BeginTable("##tablexz", 4))
                {
                    ImGui::TableNextRow(); ImGui::TableNextRow();

                    ImGui::TableNextRow(); ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 20.0f);
                    ImGui::TextInputComboBox("Name", g_Options.playername.value->mystring, 1024, opt.names, 0);
                    ImGui::InputText("Clan tag", g_Options.playerclan.value->mystring, 1024);

                    DisableElements(*g_Options.discmsg_active, 1);
                    ImGui::InputText("Disconnect msg", g_Options.discmsg.value->mystring, 1024);
                    DisableElements(*g_Options.discmsg_active, 0);

                    ImGui::TableNextColumn();
                    if (ImGui::Button("Apply##name", ImVec2(70, 22)))
                    {
                        ConVar* nameConvar = iff.g_pCVar->FindVar("name");
                        *(int*)((DWORD)&nameConvar->m_fnChangeCallbacks.m_Size) = 0;
                        printfdbg("m_fnChangeCallbacks %x\n", (DWORD)&nameConvar->m_fnChangeCallbacks.m_Size);
                        //nameConvar->SetValue(MakeControlChars(g_Options.playername.value->mystring));
                        SetName(MakeControlChars(g_Options.playername.value->mystring));
                        *g_Options.changing_name = 2;
                    }

                    if (ImGui::Button("Apply##clan", ImVec2(70, 22)))
                    {
                        char* newclan = MakeControlChars(g_Options.playerclan.value->mystring);
                        static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))FindPatternV2("engine.dll", "53 56 57 8B DA 8B F9 FF 15");
                        fnClantagChanged(newclan, newclan);
                    }

                    ImGui::Checkbox("Active##discmsg", g_Options.discmsg_active);

                    ImGui::TableNextColumn();

                    ImGui::Checkbox("Set score", g_Options.fakescore);


                    DisableElements(g_Options.fakescore, 1);

                    ImGui::InputInt("Score", g_Options.score, 0);
                    ImGui::InputInt("MVPs", g_Options.MVPs, 0);

                    ImGui::TableNextColumn();

                    ImGui::InputInt("Kills", g_Options.kills, 0);
                    ImGui::InputInt("Assists", g_Options.assists, 0);
                    ImGui::InputInt("Deaths", g_Options.deaths, 0);

                    DisableElements(g_Options.fakescore, 0);

                    ImGui::EndTable();
                }



                ImGui::Separator();


                if (ImGui::BeginTable("##tablexz", 4))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Checkbox("Set MM info", g_Options.profile_active);
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Update##ranks", ImVec2(70, 22)))
                    {
                        g_Options.rankz.value->selectedrank = 2;
                        ProtoFeatures.SendMatchmakingClient2GCHello();
                        ProtoFeatures.SendClientGcRankUpdate();
                    }
                    ImGui::EndTable();
                }

                DisableElements(g_Options.profile_active, 1);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);

                if (ImGui::BeginTable("##tablexz", 4))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    static int selectedrank_this = 0;
                    style->WindowPadding = ImVec2(5.0f, 5.0f);
                    if (ImGui::BeginCombo("Rank Type", g_Options.rankz.value->arr[selectedrank_this].Name))
                    {
                        ImGui::PushFont(ifontmini);
                        style->ItemSpacing = ImVec2(7.0f, 2.0f);
                        for (int n = 0; n < 3; n++)
                        {
                            if (ImGui::Selectable(g_Options.rankz.value->arr[n].Name, g_Options.rankz.value->arr[n].isselected, 0, ImVec2(0, 0), false))
                                selectedrank_this = n;

                            if (g_Options.rankz.value->arr[n].isselected)
                                ImGui::SetItemDefaultFocus();
                        }
                        style->ItemSpacing = ImVec2(7.0f, 15.0f);
                        ImGui::PushFont(ifont);
                        ImGui::EndCombo();
                    }
                    style->WindowPadding = ImVec2(20.f, 20.0f);

                    ImGui::InputInt("Rank", &g_Options.rankz.value->arr[selectedrank_this].rank, 0);
                    ImGui::InputInt("Wins", &g_Options.rankz.value->arr[selectedrank_this].wins, 0);

                    ImGui::TableNextColumn();

                    ImGui::Checkbox("Prime", g_Options.prime); 

                    ImGui::InputInt("level", g_Options.level, 0);
                    ImGui::InputInt("XP", g_Options.xp, 0);

                    ImGui::TableNextColumn();

                    ImGui::InputInt("Leader", g_Options.comleader, 0);
                    ImGui::InputInt("Teacher", g_Options.comteacher, 0);
                    ImGui::InputInt("Friendly", g_Options.comfriendly, 0);

                    ImGui::TableNextColumn();

                    ImGui::Checkbox("VAC ban", g_Options.vacban);
                    ImGui::InputInt("Ban reason", g_Options.banreason, 0);
                    ImGui::InputInt("Ban duration", g_Options.banduration, 0);

                    DisableElements(g_Options.profile_active, 0);
                    ImGui::EndTable();
                }


                ImGui::EndChild();
            }

            ImGui::EndTabItem();

        }


        if (ImGui::BeginTabItem("Player"))
        {
            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);

                ImGui::Text("Set ConVar");
                ImGui::SameLine();

                static char cvarbuf[255] = "";
                ImGui::PushItemWidth(200.0f);
                ImGui::InputText("##Set ConVar", cvarbuf, 255);
                ImGui::SameLine();
                static float floatbuf = 0;

                ImGui::PushItemWidth(200.0f); 
                    ImGui::InputFloat("Value##f", &floatbuf);
                ImGui::SameLine();
                if (ImGui::Button("Apply", ImVec2(70, 22)))
                {
                    SetValueUnrestricted(cvarbuf, floatbuf); 
                }

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f);
                ImGui::Separator(); 
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

                ImGui::Columns(3, nullptr, false);
                 

                if (ImGui::SliderFloat("Aspect Ratio", g_Options.aspectratio, 0, 25.0f))
                    SetValueUnrestricted("r_aspectratio", g_Options.aspectratio);
                 
                if (ImGui::Checkbox("No postprocessing", g_Options.postproc))
                    SetValueUnrestricted("mat_postprocess_enable", !g_Options.postproc);
                      

                if (ImGui::Checkbox("No shadows", g_Options.shadows)) 
                    SetValueUnrestricted("cl_csm_enabled", !g_Options.shadows); 
                 

                style->WindowPadding = ImVec2(5.0f, 5.0f); 
                static string types[3] = { "0", "1", "2" };
                if (ImGui::BeginCombo("Fullbright", types[g_Options.fullbright].c_str()))
                {
                    ImGui::PushFont(ifontmini);
                    style->ItemSpacing = ImVec2(7.0f, 2.0f);
                    for (int n = 0; n < 3; n++)
                    {
                        bool selected = (g_Options.fullbright == n);
                        if (ImGui::Selectable(types[n].c_str(), false, 0, ImVec2(0, 0), false))
                        {
                            memcpy(g_Options.fullbright, &n, 4);
                            SetValueUnrestricted("mat_fullbright", g_Options.fullbright);
                        }
                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    style->ItemSpacing = ImVec2(7.0f, 15.0f);
                    ImGui::PushFont(ifont);
                    ImGui::EndCombo();
                }
                style->WindowPadding = ImVec2(20.f, 20.0f);

                if (ImGui::Checkbox("Gray world", g_Options.drawgray))
                    SetValueUnrestricted("mat_drawgray", g_Options.drawgray);

                if (ImGui::Checkbox("Low res world", g_Options.showlowresimage))
                    SetValueUnrestricted("mat_showlowresimage", g_Options.showlowresimage);

                ImGui::NextColumn();
                 

                
                if (ImGui::SliderFloat("Viewmodel pos X", g_Options.viewmodel_x, -90.0f, 90.0f))
                    SetValueUnrestricted("viewmodel_offset_x", g_Options.viewmodel_x); 
                if (ImGui::SliderFloat("Viewmodel pos Y", g_Options.viewmodel_y, -90.0f, 90.0f))
                    SetValueUnrestricted("viewmodel_offset_y", g_Options.viewmodel_y);
                if (ImGui::SliderFloat("Viewmodel pos Z", g_Options.viewmodel_z, -90.0f, 90.0f))
                    SetValueUnrestricted("viewmodel_offset_z", g_Options.viewmodel_z);
                ImGui::SliderFloat("Viewmodel ang X", g_Options.viewmodel_ang_x, -180.0f, 180.0f);
                ImGui::SliderFloat("Viewmodel ang Y", g_Options.viewmodel_ang_y, -180.0f, 180.0f);
                ImGui::SliderFloat("Viewmodel ang Z", g_Options.viewmodel_ang_z, -180.0f, 180.0f);
                 
                ImGui::NextColumn();
                
                ImGui::SliderFloat("FOV", g_Options.fov, 0, 360); 

                if (ImGui::SliderFloat("Viewmodel FOV", g_Options.viewmodel_fov, 0, 180.0f))
                    SetValueUnrestricted("viewmodel_fov", g_Options.viewmodel_fov);


                if (ImGui::Checkbox("No viewmodel bob", g_Options.viewmodel_moving))
                {
                    if (g_Options.viewmodel_moving)
                    {
                        SetValueUnrestricted("cl_viewmodel_shift_left_amt", 0);
                        SetValueUnrestricted("cl_viewmodel_shift_right_amt", 0);
                        SetValueUnrestricted("cl_bob_lower_amt", 0);
                        SetValueUnrestricted("cl_bobamt_lat", 0);
                        SetValueUnrestricted("cl_bobamt_vert", 0);
                        SetValueUnrestricted("cl_wpn_sway_scale", 0);
                    }
                    else
                    {
                        SetValueUnrestricted("cl_viewmodel_shift_left_amt", GetVisibleFloat("cl_viewmodel_shift_left_amt"));
                        SetValueUnrestricted("cl_viewmodel_shift_right_amt", GetVisibleFloat("cl_viewmodel_shift_right_amt"));
                        SetValueUnrestricted("cl_bob_lower_amt", GetVisibleFloat("cl_bob_lower_amt"));
                        SetValueUnrestricted("cl_bobamt_lat", GetVisibleFloat("cl_bobamt_lat"));
                        SetValueUnrestricted("cl_bobamt_vert", GetVisibleFloat("cl_bobamt_vert"));
                        SetValueUnrestricted("cl_wpn_sway_scale", GetVisibleFloat("cl_wpn_sway_scale"));
                    }
                }
                 
                if (ImGui::InputFloat("Ragdoll Gravity", g_Options.ragdollgravity))
                    iff.g_pCVar->FindVar("cl_ragdoll_gravity")->SetValue(g_Options.ragdollgravity);

                if (ImGui::InputFloat("Ragdoll Timescale", g_Options.ragdolltime))
                    SetValueUnrestricted("cl_phys_timescale", g_Options.ragdolltime);


                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 67.0f);
                if (ImGui::Button("Reset", ImVec2(70, 22))) 
                    ResetMisc(); 


                ImGui::Columns(1, nullptr, false);

                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }

        /*
        if (ImGui::BeginTabItem("Movement"))
        { 
            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);

                ImGui::Columns(2, nullptr, false);

                ImGui::Checkbox("Bunny hop", g_Options.bunnyhop);
                ImGui::Checkbox("Fast duck", g_Options.fastduck);
                ImGui::Checkbox("Slide walk", g_Options.slidewalk);

                ImGui::NextColumn();

                ImGui::EndChild(); 
            }

            ImGui::EndTabItem();
        }
        */

        if (ImGui::BeginTabItem("About"))
        {
            static ImVec4 colwhite = { 0.8f, 0.8f, 0.8f, 1.0f };

            style->ChildBorderSize = 0; style->WindowPadding = ImVec2(20.0f, 5.0f);
            if (ImGui::BeginChild("ChildTab", ImVec2(665, 350), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding)) {
                style->ChildBorderSize = 1; style->WindowPadding = ImVec2(20.0f, 20.0f);

                ImGui::Columns(2, nullptr, false);

                ImGui::TextColored(colwhite, XorStr("Seaside"));
                ImGui::Text(XorStr("Build November 9, 2022"));
                ImGui::InvisibleButton("##inv", ImVec2(0, 0));
                ImGui::TextColored(colwhite, XorStr("Developer"));
                ImGui::Text(XorStr("0TheSpy"));
                ImGui::InvisibleButton("##inv", ImVec2(0, 0));
                ImGui::TextColored(colwhite, XorStr("Check for updates"));
                ImGui::Text(XorStr("unknowncheats.me"));

                ImGui::InvisibleButton("##inv", ImVec2(0, 24));
                if (ImGui::Button("Unhook", ImVec2(70, 22)))
                    opt.unhook = true;

                 
                ImGui::NextColumn();

                static int selected = 0; 
                static char name[255] = "";  

                style->ItemSpacing = ImVec2(7.0f, 2.0f);
                if (ImGui::BeginListBox("##cfgs", ImVec2(251.0f, 5 * ImGui::GetTextLineHeightWithSpacing())))
                {
                    ImGui::PushFont(ifontmini);
                    if (ImGui::BeginTable("split", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings )) 
                    {   
                            ImGui::TableSetColumnWidth(0, 10);
                            ImGui::TableSetColumnWidth(1, 80);

                        for (int n = 0; n < Config::Get().configs.size(); n++)
                        {
                            bool isselected = selected == n;

                            ImGui::TableNextRow();  
                            ImGui::TableNextColumn();
                            
                            ImGui::Text(opt.autoconfig == n ?  " *" : "  ");
                            ImGui::TableNextColumn();
                            
                            if (ImGui::Selectable(Config::Get().configs[n].c_str(), isselected, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0, 0), true)) {
                                selected = n;
                                string namewithoutss = Config::Get().configs[n].substr(0, Config::Get().configs[n].size() - 3);
                                memcpy(name, namewithoutss.c_str(), 255);
                            }
                            ImGui::TableNextColumn();
                            ImGui::Text(Config::Get().times[n].c_str());

                            if (isselected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndTable();
                    }
                    ImGui::PushFont(ifont);
                    ImGui::EndListBox();
                }
                style->ItemSpacing = ImVec2(7.0f, 15.0f);

                ImGui::InvisibleButton("##invis", ImVec2(0, 10.0f));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5.0f);

                ImGui::PushItemWidth(130.0f);
                ImGui::InputText("File name", name, 255);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                bool isloadonstartup = opt.autoconfig == selected;
                if (ImGui::Checkbox("Load on startup", &isloadonstartup)) 
                {
                    if (isloadonstartup)
                    {
                        ofstream autoload;
                        autoload.open("seaside_autoload");
                        autoload << Config::Get().configs[selected];
                        autoload.close();
                        opt.autoconfig = selected;
                    }
                    else
                    {
                        if (remove("seaside_autoload") != 0)
                            perror("Error deleting file");
                        else
                        {
                            puts("File successfully deleted");
                            opt.autoconfig = -1;



                        }
                    }
                }

                if (ImGui::BeginTable("##tablexz", 2))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                     

                    if (ImGui::Button("Create", ImVec2(70, 22)))
                    {
                        Config::Get().Create(name);

                        Config::Get().FindAutoexec();

                    }
                     
                    DisableElements(Config::Get().configs.size(), 1);
                    if (ImGui::Button("Save", ImVec2(70, 22)))
                        Config::Get().Save(selected, name);
                    DisableElements(Config::Get().configs.size(), 0);

                    if (ImGui::Button("Refresh", ImVec2(70, 22)))
                    {
                        Config::Get().Refresh();
                        Config::Get().FindAutoexec();
                    }

                    ImGui::TableNextColumn();

                    DisableElements(Config::Get().configs.size(), 1);

                    if (ImGui::Button("Delete", ImVec2(70, 22)))
                    {
                        Config::Get().Delete(selected);
                        if (selected >= Config::Get().configs.size()) selected = 0;
                        if (selected == opt.autoconfig) opt.autoconfig = -1;
                         
                        Config::Get().FindAutoexec();
                    }
                     
                    if (ImGui::Button("Load", ImVec2(70, 22)))
                        Config::Get().Load(selected);
                    DisableElements(Config::Get().configs.size(), 0);
                     
                    if (ImGui::Button("Folder", ImVec2(70, 22)))
                        Config::Get().OpenFolder();

                    ImGui::EndTable();
                }
                ImGui::Columns(1, nullptr, false);

                ImGui::TextColored(colwhite, "                      Author does not take any responsibility for bans caused by this software");
                ImGui::TextColored(colwhite, "                                         Please use VAC Bypass for better protection");

                ImGui::EndChild();
            }


            ImGui::EndTabItem();
        } 
        ImGui::EndTabBar(); 
        ImGui::End(); 
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
     
    if (*g_Options.hitmarker && iff.g_pEngineClient->IsInGame()) {
        iff.g_pDebugOverlay->ClearAllOverlays();
        for (int i = 0; i < bulletdata.size(); i++) {
            Vector pos = bulletdata[i].pos;
            float delta = bulletdata[i].time - bulletdata[i].curtime;
            pos.z += delta * 35;
            int alpha = 255;
            if (bulletdata[i].time >= bulletdata[i].curtime + 0.70f)
                alpha = int(850.0f * (-(delta - 1.0f)));
            if (alpha < 0) alpha = 0;
            if (!bulletdata[i].hs)
                iff.g_pDebugOverlay->AddTextOverlayRGB(pos, 0, 0.09, 255, 50, 50, alpha, std::to_string(bulletdata[i].dmg).c_str());
            else
                iff.g_pDebugOverlay->AddTextOverlayRGB(pos, 0, 0.09, 0, 255, 0, alpha, std::to_string(bulletdata[i].dmg).c_str());
            if (delta >= 1.000f)
                bulletdata.erase(bulletdata.begin() + i);
            bulletdata[i].time += iff.g_pGlobals->curtime - bulletdata[i].time;
        }
    } 
    return oEndScene(pDevice);
}
