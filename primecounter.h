#ifndef PRIMECOUNTER_H
#define PRIMECOUNTER_H

#include <QObject>
#include <QFutureWatcher>
#include <QElapsedTimer>
#include <QtConcurrent>
#include <numeric>


class PrimeCounter : public QObject
{
    Q_OBJECT
    using Element = unsigned long long;
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString resultText READ resultText NOTIFY resultTextChanged)
    Q_PROPERTY(QString filterText READ filterText NOTIFY filterTextChanged)
public:
    explicit PrimeCounter(QObject *parent = nullptr);

    ~PrimeCounter();

    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void changeCurrentReducedOptions(QString reducedOption);
    Q_INVOKABLE void setStepSize(double newStepSize);
    Q_INVOKABLE void fillElements(unsigned int count);
    int progress() const;
    QString resultText() const;
    QString filterText() const;

    void finish();

signals:
    void progressChanged();
    void resultTextChanged();
    void filterTextChanged();

    void started();
    void done();
    void calculating();

private:
    unsigned int stepSize;
    QFutureWatcher<Element> watcher;
    QElapsedTimer timer;
    QList<Element> elementList;
    QString m_resultText;
    QString m_filterText;
    int m_progress;
    QThreadPool pool;
    QtConcurrent::ReduceOptions currentReducedOpts;

    static void reduceFunction(Element &out, const Element &value);
    static bool filterFunction(const Element &element);
};


#endif // PRIMECOUNTER_H



















