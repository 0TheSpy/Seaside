#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <string>
#include <vector>

extern ImFont* ifontmini;
extern ImFont* ifont;



namespace ImGui {

	bool identical(const char* buf, const char* item) {
		size_t buf_size = strlen(buf);
		size_t item_size = strlen(item);
		if (buf_size >= item_size) return false;
		for (int i = 0; i < strlen(buf); ++i)
			if (buf[i] != item[i]) return false;
		return true;
	};

	int propose(ImGuiInputTextCallbackData* data) {
		if (strlen(data->Buf) == 0) return 0;

		const char** items = static_cast<std::pair<const char**, size_t>*> (data->UserData)->first;
		size_t length = static_cast<std::pair<const char**, size_t>*> (data->UserData)->second;

		if (IsKeyPressedMap(ImGuiKey_Backspace)) {     
			if (data->SelectionEnd != data->SelectionStart)
				if (data->BufTextLen > 0)     			
					if (data->CursorPos > 0)       
						data->DeleteChars(data->CursorPos - 1, 1);
			return 0;
		}
		if (IsKeyPressedMap(ImGuiKey_Delete)) return 0;     



		for (int i = 0; i < length; i++) {
			if (identical(data->Buf, items[i])) {
				const int cursor = data->CursorPos;
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, items[i]);
				data->CursorPos = cursor;
				data->SelectionStart = cursor;
				data->SelectionEnd = data->BufTextLen;
				break;
			}
		}
		return 0;
	}



	bool TextInputComboBox(const char* id, char* buffer, size_t maxInputSize, const char* items[], size_t item_len, short showMaxItems) {
		if (showMaxItems == 0)
			showMaxItems = item_len;

		ImGui::PushID(id);
		std::pair<const char**, size_t> pass(items, item_len);         

		bool ret = ImGui::InputText("##in", buffer, maxInputSize, 0, 0, 0);

		ImGui::OpenPopupOnItemClick("combobox");  
		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();

		ImGui::SameLine(0, 0);
		if (ImGui::ArrowButton("##openCombo", ImGuiDir_Down)) {
			ImGui::OpenPopup("combobox");
		}
		ImGui::OpenPopupOnItemClick("combobox");  

		pos.y += size.y;
		size.x += ImGui::GetItemRectSize().x;
		size.y += 5 + (size.y * showMaxItems);
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		ImGuiStyle* style = &ImGui::GetStyle();
		ImGui::PushFont(ifontmini);
		style->ItemSpacing = ImVec2(7.0f, 2.0f); 
		style->WindowPadding = ImVec2(5.0f, 5.0f);

		if (ImGui::BeginPopup("combobox", ImGuiWindowFlags_::ImGuiWindowFlags_NoMove)) {


			for (int i = 0; i < item_len; i++)
				if (ImGui::Selectable(items[i]))
					strcpy(buffer, items[i]);




			ImGui::EndPopup();
		}

		style->WindowPadding = ImVec2(20.f, 20.0f);
		style->ItemSpacing = ImVec2(7.0f, 15.0f); 
		ImGui::PushFont(ifont);

		ImGui::PopID();

		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
		return ret;
	}















































	bool TextInputComboBox(const char* id, char* buffer, size_t maxInputSize, std::vector<std::string> items, short showMaxItems) {

		const ImVec2 label_size = CalcTextSize(id, NULL, true);
		if (label_size.x > 0) {
			ImVec2 savecursor = ImGui::GetCursorPos(); 
			ImVec2 newcursor = savecursor;
			savecursor.y += 20.0f;
			ImGui::SetCursorPos(newcursor);
			ImGui::TextEx(id);
			ImGui::SetCursorPos(savecursor);
		}

		size_t item_len = items.size();

		if (showMaxItems == 0)
			showMaxItems = item_len;

		ImGui::PushID(id);
		std::pair<std::vector<std::string>, size_t> pass(items, item_len);         

		ImGuiStyle* style = &ImGui::GetStyle();

		const ImVec2 frame_size = CalcItemSize(ImVec2(0, 0), CalcItemWidth(),  label_size.y + style->FramePadding.y * 2.0f);         

		ImGui::SetNextItemWidth(frame_size.x - 19.0f);

		bool ret = ImGui::InputText("##in", buffer, maxInputSize, 0, 0, 0);

		ImGui::OpenPopupOnItemClick("combobox");  
		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();

		ImGui::SameLine(0, 0);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5.0f);
		if (ImGui::ArrowButton("##openCombo", ImGuiDir_Down)) {
			ImGui::OpenPopup("combobox");
		}
		ImGui::OpenPopupOnItemClick("combobox");  

		pos.y += size.y;


		ImGui::SetNextWindowPos(pos);
		size.x += 15.0f;
		ImGui::SetNextWindowSizeConstraints(size, ImVec2(400.0f, 200.0f));

		ImGui::PushFont(ifontmini);
		style->ItemSpacing = ImVec2(7.0f, 2.0f); 
		style->WindowPadding = ImVec2(5.0f, 5.0f);

		if (ImGui::BeginPopup("combobox", ImGuiWindowFlags_::ImGuiWindowFlags_NoMove)) {

			for (int i = 0; i < item_len; i++)
				if (ImGui::Selectable(items[i].c_str()))
					strcpy(buffer, items[i].c_str());
			ImGui::EndPopup();
		}

		style->WindowPadding = ImVec2(20.f, 20.0f);
		style->ItemSpacing = ImVec2(7.0f, 15.0f); 
		ImGui::PushFont(ifont);

		ImGui::PopID();


		return ret;
	}










}