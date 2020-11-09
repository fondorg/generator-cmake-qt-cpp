
#ifndef HL_NORM_MAIN_WINDOW_H
#define HL_NORM_MAIN_WINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QFuture>
#include <QFutureWatcher>
#include <QVariant>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include <QLabel>
#include <QSettings>
#include <QThread>
#include <QIntValidator>
#include <QMessageBox>
#include <QList>
#include <QLineEdit>
#include <QFileDialog>
#include <QIntValidator>
<% if (opencv) { %>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
<% } %>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    QProgressBar *bar;
    QLabel *statusLabel;

    QSettings *settings_;

    /**
     * Reusable watcher to fetch background tasks data
     */
    QFutureWatcher<QVariant> watcher_;

    template<typename T, typename Class>
    void doBg(T (Class::*fn)(), const char *slot) {
        connect(this, SIGNAL(bgFinished()), this, slot);
        connect(&watcher_, SIGNAL(finished()), this, SLOT(whenBgFinished()), Qt::QueuedConnection);
        QFuture<QVariant> f = QtConcurrent::run(this, fn);
        watcher_.setFuture(f);
    };

    /**
     * Same as doBg(T (Class::*fn)(), const char *slot) but this one is blocking UI with the provided message
     * @tparam T
     * @tparam Class
     * @param fn
     * @param slot
     */
    template<typename T, typename Class>
    void doBg(T (Class::*fn)(), const char *slot, const QString &msg, bool dontBlock = false) {
        if (!dontBlock) {
            blockUi(msg);
        }
        connect(this, SIGNAL(bgFinished()), this, slot);
        connect(&watcher_, SIGNAL(finished()), this, SLOT(whenBgFinished()), Qt::QueuedConnection);
        QFuture<QVariant> f = QtConcurrent::run(this, fn);
        watcher_.setFuture(f);
    };

    void blockUi(const QString &statusText);

    void msg(const QString &text, QMessageBox::Icon icon = QMessageBox::NoIcon);

    void updateSettings(const QString &key, const QString &value);

    void testBtnClick();

    QVariant testBtn();

    /**
     * Select existing directory in the file system
     * @param field field to fill in the selected directory absolute path
     */
    void selectDir(QLineEdit *field);

    /**
     * Select existing file in the file system
     * @param field field to fill in the selected file absolute path
     */
    void selectFile(QLineEdit *field);

private slots:

    void releaseUi();

    void whenBgFinished();


signals:

    void bgFinished();
};


#endif //HL_NORM_MAIN_WINDOW_H
