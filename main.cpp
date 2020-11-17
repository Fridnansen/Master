#include "mainwindow.h"
#include <QApplication>
#include "myopengl.h"

//=================================================================
#include <tlhelp32.h>

bool isProcessRunning(const char *ProcName) {
  HANDLE SnapHandle;
  PROCESSENTRY32 pe;

  SnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  pe.dwSize = sizeof(PROCESSENTRY32);

  Process32First(SnapHandle, &pe);

  while (Process32Next(SnapHandle, &pe)) {
    setlocale(LC_ALL, "en_US.utf8");
    char cExeFile[MAX_PATH];
    std::wcstombs(cExeFile, pe.szExeFile, MAX_PATH);
    if (!strcmp(cExeFile, ProcName)) {
      return true;
    }
  }

  CloseHandle(SnapHandle); // Snapshot Handle wird geschlossen.
  return false;
}

void cargar_video() {

  QString wmplayer = "C:/Program Files/Windows Media Player/wmplayer.exe";
  QString fileName = "C:/Users/First Vision/Desktop/video.wmv";

  QStringList arguments;
  arguments << fileName << "/fullscreen";

  QProcess *myProcess = new QProcess();
  if (!myProcess)
    return;

  myProcess->start(wmplayer, arguments, QIODevice::ReadOnly);

  if (myProcess->waitForStarted()) {
    for (DWORD dw = 0; dw < 102; dw++) {
      Sleep(1000);
      if (!isProcessRunning("wmplayer.exe")) {
        break;
      }
    }
    myProcess->close();
  }
}

//=================================================================

int main(int argc, char *argv[]) {

  cargar_video();
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();
  return a.exec();
}
