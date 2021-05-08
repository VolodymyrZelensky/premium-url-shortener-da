#ifndef MW_H
#define MW_H
#define UNUSED(expr) do { (void)(expr); } while (0)

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QFont>
#include <QFontDatabase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QDebug>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui { class mw; }
QT_END_NAMESPACE

class mw : public QMainWindow
{
		Q_OBJECT

	public:
		mw(QWidget *parent = nullptr);
		~mw();

	private slots:
		void on_btn_short_clicked();

		void on_btn_copy_clicked();

		void on_btn_open_link_clicked();

		bool AreYouOnline();

		void finished(QNetworkReply *reply);

		void Progress(qint64 received, qint64 total);

		void on_btn_developer_clicked();

	private:
		Ui::mw *ui;
};
#endif // MW_H
