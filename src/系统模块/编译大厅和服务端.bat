@set path=D:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE;%path%

@cls

devenv "�������\��������\ComService.vcproj" /build release
devenv "�������\����ؼ�\SkinControls.vcproj" /build release
devenv "�������\�������\SocketModule.vcproj" /build release
       
devenv "�ͻ������\�ͻ��˹���\ClientShare.vcproj" /build release
devenv "�ͻ������\�ŵ�ģ��\ChannelModule.vcproj" /build release
devenv "�ͻ������\��ϵ����\Companion.vcproj" /build release
devenv "�ͻ������\ͷ�����\UserFace.vcproj" /build release
devenv "�ͻ������\�������\DownLoad.vcproj" /build release
devenv "�ͻ������\��Ϸ�ȼ�\GameRank.vcproj" /build release
       
devenv "�ͻ������\(��ʾ)��Ϸ�㳡\GamePlaza.vcproj" /build release
devenv "�ͻ������\��Ϸ���\GameFrame.vcproj" /build release
       
       
devenv "���������\��½������\LogonServer.vcproj" /build release
devenv "���������\����װ����\ServiceLoader.vcproj" /build release
devenv "���������\�б����\ListService.vcproj" /build release
devenv "���������\�ں����\ServerKernel.vcproj" /build release
devenv "���������\��Ϸ����\GameService.vcproj" /build release
devenv "���������\���ķ�����\CenterServer.vcproj" /build release

pause;

