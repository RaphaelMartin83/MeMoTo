#include <QApplication>
#include <QClipboard>

#include "ErrorDisplayer.h"
#include "ConfigWidget.h"

QGridLayout* ErrorDisplayer::m_Layout = nullptr;
QPushButton* ErrorDisplayer::m_CopyButton = nullptr;
QPushButton* ErrorDisplayer::m_OKButton = nullptr;
QLabel* ErrorDisplayer::m_NameLabel = nullptr;
QLabel* ErrorDisplayer::m_Text = nullptr;

static ErrorDisplayer* s_Me = nullptr;

ErrorDisplayer::ErrorDisplayer()
{
    m_Layout = new QGridLayout();
    m_CopyButton = new QPushButton();
    m_OKButton = new QPushButton();
    m_NameLabel = new QLabel();
    m_Text = new QLabel();

    m_CopyButton->setText("Copy Error");
    m_OKButton->setText("OK");
    m_NameLabel->setFixedHeight(30);

    // Setup layout
    unsigned short l_row = 0U;

    m_Layout->addWidget(m_NameLabel, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_Text, l_row++, 0, 1, 1, Qt::Alignment());
    m_Layout->addWidget(m_CopyButton, l_row++, 0, 1, 1, Qt::AlignBottom);
    m_Layout->addWidget(m_OKButton, l_row++, 0, 1, 1, Qt::AlignBottom);

    this->setLayout(m_Layout);

    connect(m_OKButton, &QPushButton::clicked, this,
            &ErrorDisplayer::OKPressed);
    connect(m_CopyButton, &QPushButton::clicked, this,
            &ErrorDisplayer::CopyPressed);
}

void ErrorDisplayer::displayError(const QString& p_ErrorTitle,
                                  const QString& p_ErrorText)
{
    if( nullptr == s_Me )
    {
        s_Me = new ErrorDisplayer();
    }

    m_NameLabel->setText(p_ErrorTitle);
    m_Text->setText(p_ErrorText);
    m_Text->setWordWrap(true);

    ConfigWidget::open(s_Me);
}

void ErrorDisplayer::setFocusOnFirstZone()
{
    m_OKButton->setFocus();
}

void ErrorDisplayer::OKPressed()
{
    ConfigWidget::close();
}

void ErrorDisplayer::CopyPressed()
{
    QClipboard* l_pClipboard = QApplication::clipboard();
    l_pClipboard->clear(QClipboard::Clipboard);
    l_pClipboard->setText(m_Text->text(), QClipboard::Clipboard);
}
