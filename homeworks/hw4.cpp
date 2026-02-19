#include <iostream>
#include <string>
#include <memory>
#include <queue>

class Document {
private:
    std::string m_content;

public:
    Document() {}
    Document(const std::string& text) : m_content(text) {}

    void insert(size_t pos, const std::string& str) {
        m_content.insert(pos, str);
    }

    void erase(size_t pos, size_t count) {
        m_content.erase(pos, count);
    }

    void replace(size_t pos, size_t count, const std::string& str) {
        m_content.replace(pos, count, str);
    }

    const std::string& getText() const {
        return m_content;
    }
};

class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};


class InsertTextCommand : public Command {
private:
    std::weak_ptr<Document> m_doc;
    std::string m_text;
    size_t m_position;

public:
    InsertTextCommand(std::shared_ptr<Document> doc,
                      const std::string& text,
                      size_t position)
        : m_doc(doc), m_text(text), m_position(position) {}

    void execute() override {
        std::shared_ptr<Document> doc = m_doc.lock();

        if (!doc) {
            std::cout << "Document not found\n";
            return;
        }

        doc->insert(m_position, m_text);
    }
};


class EraseTextCommand : public Command {
private:
    std::weak_ptr<Document> m_doc;
    size_t m_position;
    size_t m_count;

public:
    EraseTextCommand(std::shared_ptr<Document> doc,
                     size_t position,
                     size_t count)
        : m_doc(doc), m_position(position), m_count(count) {}

    void execute() override {
        std::shared_ptr<Document> doc = m_doc.lock();

        if (!doc) {
            std::cout << "Document not found\n";
            return;
        }

        doc->erase(m_position, m_count);
    }
};

class ReplaceTextCommand : public Command {
private:
    std::weak_ptr<Document> m_doc;
    size_t m_position;
    size_t m_count;
    std::string m_text;

public:
    ReplaceTextCommand(std::shared_ptr<Document> doc,
                       size_t position,
                       size_t count,
                       const std::string& text)
        : m_doc(doc), m_position(position),
          m_count(count), m_text(text) {}

    void execute() override {
        std::shared_ptr<Document> doc = m_doc.lock();

        if (!doc) {
            std::cout << "Document not found\n";
            return;
        }

        doc->replace(m_position, m_count, m_text);
    }
};


class CommandScheduler {
private:
    std::queue<std::unique_ptr<Command>> m_pending;

public:
    void schedule(std::unique_ptr<Command> cmd) {
        m_pending.push(std::move(cmd));
    }

    void runAll() {
        while (!m_pending.empty()) {
            std::unique_ptr<Command> cmd = std::move(m_pending.front());
            m_pending.pop();

            if (cmd) {
                cmd->execute();
            }
        }
    }
};


int main() {

    auto doc1 = std::make_shared<Document>("Hello");
    auto doc2 = std::make_shared<Document>("World");

    CommandScheduler scheduler;

    std::cout << "Initial state:\n";
    std::cout << "doc1: " << doc1->getText() << std::endl;
    std::cout << "doc2: " << doc2->getText() << std::endl;
    std::cout << std::endl;


    scheduler.schedule(std::make_unique<InsertTextCommand>(doc1, " world", 5));
    scheduler.schedule(std::make_unique<InsertTextCommand>(doc2, "!!!", 5));
    scheduler.schedule(std::make_unique<ReplaceTextCommand>(doc1, 6, 5, "C++"));

    {
        auto tempDoc = std::make_shared<Document>("Temporary");
        scheduler.schedule(
            std::make_unique<InsertTextCommand>(tempDoc, " text", 9)
        );
    } 

    std::cout << "Running commands...\n\n";

    scheduler.runAll();

    std::cout << "Final state:\n";
    std::cout << "doc1: " << doc1->getText() << std::endl;
    std::cout << "doc2: " << doc2->getText() << std::endl;

    return 0;
}
