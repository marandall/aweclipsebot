function EventAvatarAdd(av)
{
  icon = Evo2.TrayIcon.CreateCis(
            "jc",							# the command to call on click
            "hud_0jc.png",		# which image to use
            "jgr",            # a key to help identify
            20);              # index within the list 
	av.addTrayIcon(icon);
}

function EventChannelConnectionAdd(av, channel)
{
	DisplayChannelMembers(channel);

	// handle join / disconnect
	icon = av.getTrayIconByKey("jgr");
	if (icon != null && channel.name == "#world")
	{
		icon.click_function = "qc";
		icon.text = "hud_0qc.png";

		// cause the bar to rebuild
		av.updateTray();
	}
}

function EventChannelConnectionDelete(av, channel)
{
	DisplayChannelMembers(channel);

	// handle join / disconnect
	icon = av.getTrayIconByKey("jgr");
	if (icon != null && channel.name == "#world")
	{
		icon.click_function = "jc";
		icon.text = "hud_0jc.png";

		// cause the bar to rebuild
		av.updateTray();
	}
}