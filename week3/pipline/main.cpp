#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <deque>

using namespace std;


struct Email {
    string from;
    string to;
    string body;
};

istream& operator >> (istream& in, Email& email) {

    getline(in, email.from);
    getline(in, email.to);
    getline(in, email.body);
    return in;
}

ostream& operator << (ostream& out, const Email& email) {
    out << email.from << "\n" << email.to << "\n" << email.body;
    return out;
}

bool operator == (const Email& lhs, const Email& rhs) {
    return tie(lhs.from, lhs.to, lhs.body) ==  tie(rhs.from, rhs.to, rhs.body);
}

class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_) {
            next_->Process(move(email));
        }

    }

public:
    void SetNext(unique_ptr<Worker> next) {
        next_ = move(next);
    }

    unique_ptr<Worker>* GetNext() {
        return &next_;
    }
protected:
    unique_ptr<Worker> next_;
};


class Reader : public Worker {
public:
    explicit Reader(istream& in)
        : input_stream(in)
    {}

    void Process(unique_ptr<Email> email) override {
        Run();
    }

    void Run() override {
        for (Email email; input_stream >> email;) {

            PassOn(make_unique<Email>(email));
        }
    }
private:
    istream& input_stream;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
public:
    // реализуйте класс
    explicit Filter(Function unary_predicate)
        : predicate_(move(unary_predicate)) {}

    void Process(unique_ptr<Email> email) override {
        if (predicate_(*email)) {
            PassOn(move(email));
        }
    }
private:
    Function predicate_;
};


class Copier : public Worker {
public:
    // реализуйте класс
    explicit Copier(const string& target)
        : dest_(target){}

    void Process(unique_ptr<Email> email) override {

        if (email->to != dest_) {
            unique_ptr<Email> duplicate =
                    move(DuplicateEmailWithChaingingDest(email, dest_));
            PassOn(move(email));
            PassOn(move(duplicate));
        } else {
            PassOn(move(email));
        }

    }

private:
    unique_ptr<Email> DuplicateEmailWithChaingingDest(unique_ptr<Email>& email, const string& new_dest) {
        Email copied = CopyEmail(email);
        copied.to = dest_;
        return make_unique<Email>(copied);
    }

    Email CopyEmail(unique_ptr<Email>& mail) {
        return *mail.get();
    }

private:
     string dest_;
};


class Sender : public Worker {
public:
    // реализуйте класс
    explicit Sender(ostream& out)
        : outstream_(out) {}

    void Process (unique_ptr<Email> email) override {
        outstream_ << *email << "\n";
        PassOn(move(email));
    };
private:
    ostream& outstream_;

};

//TODO: pipline_builder


// реализуйте класс
// Односвязный список
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in)
        : front(make_unique<Reader>(in)), tail(&front)
        {}

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        unique_ptr<Worker> filter_ptr = make_unique<Filter>(filter);
        PushBack(move(filter_ptr));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        unique_ptr<Worker> copier_ptr = make_unique<Copier>(recipient);
        PushBack(move(copier_ptr));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        unique_ptr<Worker> sender_ptr = make_unique<Sender>(out);
        PushBack(move(sender_ptr));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(front);
    }
private:
    void PushBack(unique_ptr<Worker> worker) {
        tail->get()->SetNext(move(worker));
        tail = tail->get()->GetNext();
    }
private:
    unique_ptr<Worker> front;
    unique_ptr<Worker>* tail;
};


void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());

    //std::cout << outStream.str() << std::endl;
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}