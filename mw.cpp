#include "mw.h"
#include "ui_mw.h"

mw::mw(QWidget *parent) : QMainWindow(parent), ui(new Ui::mw){
	ui->setupUi(this);
	QFontDatabase::addApplicationFont(":/fonts/assets/Tajawal-Regular.ttf");
	ui->label->setFont(QFont("tajawal", 20, QFont::Normal));
	ui->btn_short->setFont(QFont("tajawal", 12, QFont::Normal));
	ui->btn_copy->setFont(QFont("tajawal", 12, QFont::Normal));
	ui->btn_developer->setFont(QFont("tajawal", 10, QFont::Normal));
	ui->long_url->setFont(QFont("tajawal", 15, QFont::Normal));
	ui->short_url->setFont(QFont("tajawal", 12, QFont::Normal));
	ui->outpanel->hide();
}

mw::~mw(){delete ui;}
void mw::on_btn_open_link_clicked(){QDesktopServices::openUrl(QUrl(ui->short_url->text(), QUrl::TolerantMode));}
void mw::on_btn_developer_clicked(){QDesktopServices::openUrl(QUrl("https://twitter.com/anasybal", QUrl::TolerantMode));}

bool mw::AreYouOnline(){
	if(QProcess::execute("ping -n 1 www.google.com") == 0) return true;
	return false;
}

void mw::on_btn_copy_clicked(){
	ui->short_url->selectAll();
	ui->short_url->copy();
}

void mw::on_btn_short_clicked()
{
	if(ui->long_url->text() == nullptr){
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Information);
		messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
		messageBox.setWindowTitle("empty value");
		messageBox.setText("please insert your long url and try again...");
		messageBox.exec();
		return;
	}
	if(!ui->long_url->text().startsWith("http://")){
		if(!ui->long_url->text().startsWith("https://")){
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
			messageBox.setWindowTitle("link validation");
			messageBox.setText("please insert real link and try again...");
			messageBox.setDetailedText("the real link mean's url started with http:// or https:// and have domain name with .'tld' in the end");
			messageBox.exec();
			return;
		}
	}
	if(ui->long_url->text().split("://")[1].split("/")[0].split(".").size() >= 2){
		if(ui->long_url->text().split("://")[1].split("/")[0].split(".")[1] == "" || ui->long_url->text().split("://")[1].split("/")[0].split(".")[0] == ""){
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
			messageBox.setWindowTitle("link validation");
			messageBox.setText("please insert real link and try again...");
			messageBox.setDetailedText("the real link mean's url started with http:// or https:// and have domain name with .'tld' in the end");
			messageBox.exec();
			return;
		}
	}else{
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Information);
		messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
		messageBox.setWindowTitle("link validation");
		messageBox.setText("please insert real link and try again...");
		messageBox.setDetailedText("the real link mean's url started with http:// or https:// and have domain name with .'tld' in the end");
		messageBox.exec();
		return;
	}
	if(!this->AreYouOnline()){
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Warning);
		messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
		messageBox.setWindowTitle("Network Connection");
		messageBox.setText("please connect your network and try again...");
		messageBox.exec();
		return;
	}
	QUrl url("https://2u.pw/shorten");
	QUrlQuery params;
	params.addQueryItem("url", ui->long_url->text());
	params.addQueryItem("urls", "");
	params.addQueryItem("type", "");
	params.addQueryItem("custom", "");
	params.addQueryItem("expiry", "");
	params.addQueryItem("password", "");
	params.addQueryItem("description", "");
	params.addQueryItem("multiple", "0");
	QNetworkRequest request(url);
	request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0");
	request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
	request.setRawHeader("Accept-Language", "en-US,en;q=0.5");
	request.setRawHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.setRawHeader("X-Requested-With", "XMLHttpRequest");
	request.setRawHeader("Alt-Used", "2u.pw");
	request.setRawHeader("Connection", "keep-alive");
	request.setRawHeader("Referer", "https://2u.pw/user");
	request.setRawHeader("Origin", "https://2u.pw");
	request.setRawHeader("Cookie", "__cfduid=de1a98895ec252cd96e901675332787341620225210; _ga=GA1.2.847265522.1620225227; _gid=GA1.2.454665368.1620225227; cookieconsent_status=dismiss; login=eyJsb2dnZWRpbiI6dHJ1ZSwia2V5IjoiJDJhJDA4JFwvZkVGZUNGMkJ0TzVILnloMWhZZDBlaXNhbHAuSXdPckQyeFVtT01qbVZLeFU1ZUFTTDBUNjEwNzU4In0%3D; lang=en; PHPSESSID=0ed4b9b7336af441164522ff3d810abc; _gat_gtag_UA_121172030_1=1");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QNetworkReply *reply = manager->post(request, params.query().toUtf8());
	connect(manager, &QNetworkAccessManager::finished, this, &mw::finished);
	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(Progress(qint64, qint64)));
}

void mw::Progress(qint64 received, qint64 total){
	if(received != 0 && total != 0){
		qint64 percent;
		percent = received * 100 / total;
		ui->progress->setValue(percent);
	}
}

void mw::finished(QNetworkReply *reply){
	ui->progress->setValue(0);
	QByteArray response = reply->readAll();
	QJsonDocument data = QJsonDocument::fromJson(response);
	QJsonObject JON = data.object();
	if(JON.value("error").toInt() == 0){
		ui->outpanel->show();
		ui->short_url->setText(JON.value("short").toString());
	}else{
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Warning);
		messageBox.setWindowIcon(QIcon(":/images/assets/favicon.ico"));
		messageBox.setWindowTitle("error : " + JON.value("error").toString());
		messageBox.setText(JON.value("msg").toString());
		messageBox.exec();
	}
}
