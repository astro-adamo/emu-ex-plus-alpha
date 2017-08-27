#pragma once

/*  This file is part of EmuFramework.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with EmuFramework.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/gui/TableView.hh>
#include <imagine/gui/AlertView.hh>
#include <emuframework/EmuInput.hh>
class InputManagerView;

class ButtonConfigSetView : public View
{
private:
	typedef DelegateFunc<void (Input::Event e)> SetDelegate;

	IG::WindowRect viewFrame{};
	#ifdef CONFIG_INPUT_POINTING_DEVICES
	IG::WindowRect unbindB{}, cancelB{};
	#endif
	std::array<char, 128> str{};
	std::array<char, 24> actionStr{};
	Gfx::Text text{};
	#ifdef CONFIG_INPUT_POINTING_DEVICES
	Gfx::Text unbind{}, cancel{};
	#endif
	SetDelegate onSetD{};
	const Input::Device *dev{};
	const Input::Device *savedDev{};
	InputManagerView &rootIMView;

	void initPointerUI();
	bool pointerUIIsInit();

public:
	ButtonConfigSetView(ViewAttachParams attach, InputManagerView &rootIMView):
		View{attach}, rootIMView{rootIMView}
	{}
	~ButtonConfigSetView();
	IG::WindowRect &viewRect() override { return viewFrame; }
	void init(Input::Device &dev, const char *actionName, SetDelegate onSet);
	void place() override;
	bool inputEvent(Input::Event e) override;
	void draw() override;
	void onAddedToController(Input::Event e) override;
};

class ButtonConfigView : public TableView
{
private:
	struct BtnConfigMenuItem : public DualTextMenuItem
	{
		using DualTextMenuItem::DualTextMenuItem;
		void draw(Gfx::Renderer &r, Gfx::GC xPos, Gfx::GC yPos, Gfx::GC xSize, Gfx::GC ySize, _2DOrigin align, const Gfx::ProjectionPlane &projP) const override;
	};

	InputManagerView &rootIMView;
	TextMenuItem reset{};
	using KeyNameStr = std::array<char, 20>;
	struct BtnConfigEntry
	{
		BtnConfigMenuItem item{nullptr, ""};
		KeyNameStr str{};

		BtnConfigEntry()
		{
			item.t2.setString(str.data());
		}
	} *btn{};
	const KeyCategory *cat{};
	InputDeviceConfig *devConf{};
	Input::Time leftKeyPushTime{};

	void onSet(Input::Event e, int keyToSet);
	static KeyNameStr makeKeyNameStr(Input::Key key, const char *name);

public:
	ButtonConfigView(ViewAttachParams attach, InputManagerView &rootIMView, const KeyCategory *cat, InputDeviceConfig &devConf);
	~ButtonConfigView() override;
	bool inputEvent(Input::Event e) override;
};
