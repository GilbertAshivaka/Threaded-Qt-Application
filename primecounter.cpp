#include "primecounter.h"
#include <QThread>

PrimeCounter::PrimeCounter(QObject *parent)
    : QObject{parent}
    , stepSize(1000000)
    ,m_progress(0)
{
    connect(&watcher, QFutureWatcher<Element>::progressValueChanged, this, [this](int value){
        m_progress = value;

        emit progressChanged();
    });

    connect(&watcher, QFutureWatcher<Element>::finished, this, &PrimeCounter::finish);


}

PrimeCounter::~PrimeCounter()
{
    watcher.cancel();
}

void PrimeCounter::start()
{
    if (watcher.isRunning()){
        watcher.cancel();
        return; // does not continue if the user tries starting the prime counting when another one is still running
    }
    emit started();
    m_filterText = "Calculating...";
    emit filterTextChanged();

    timer.start();
    fillElements(stepSize);
    watcher.setFuture(QtConcurrent::filteredReduced(&pool,elementList, filterFunction, reduceFunction, currentReducedOpts | QtConcurrent::SequentialReduce));

}

void PrimeCounter::cancel()
{
    watcher.cancel();
}

void PrimeCounter::changeCurrentReducedOptions(QString reducedOption)
{
    if (reducedOption == "Unodered reduced"){
        currentReducedOpts = QtConcurrent::UnorderedReduce;
    }else{
        currentReducedOpts = QtConcurrent::OrderedReduce;
    }
}

void PrimeCounter::setStepSize(double newStepSize)
{
    stepSize = newStepSize;
}


int PrimeCounter::progress() const
{
    return m_progress;
}

QString PrimeCounter::resultText() const
{
    return m_resultText;
}

QString PrimeCounter::filterText() const
{
    return m_filterText;
}

void PrimeCounter::finish()
{
    if(watcher.isCanceled()){
        m_filterText = "Operation cancelled";
        emit done();
    }else{
        m_resultText = QString("Found %1 primes").arg(watcher.result());
        m_filterText = QString("Operation took %1 ms").arg(timer.elapsed());
    }
    emit filterTextChanged();
    emit resultTextChanged();
    emit done();
}

void PrimeCounter::reduceFunction(Element &out, const Element &value)
{
    Q_UNUSED(value);
    ++out;
}


bool PrimeCounter::filterFunction(const Element &element)
{
    if(element < 1)
        return false;
    for(Element i = 2; i*i <= element; ++i){
        if(element % i == 0){
            return false;
        }
    }
    return true;
}

void PrimeCounter::fillElements(unsigned int count)
{
    auto prevSize = elementList.size();
    if(elementList.size() == count)
        return;

    auto startVal = elementList.empty() ? 1 : elementList.back()+1;
    elementList.resize(count);
    if(elementList.begin()+prevSize < elementList.end()){
        std::iota(elementList.begin()+prevSize, elementList.end(), startVal);
    }

}




















