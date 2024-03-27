#ifndef __SIGHAND_H__
#define __SIGHAND_H__
/*******************************************************************************
 * Copyright (c) 2001-2020 Rohit Priyadarshi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * https://opensource.org/licenses/MIT
 ******************************************************************************/

namespace utl {

#define ENABLESH 1
const int cMaxSignals = 10;
const int cMaxFile = 2048;
const int cBadVal = -1;

//////////////////////////////////////////////////////////////////////
// class to keep track of all the signals assigned or tripped. 
//////////////////////////////////////////////////////////////////////
class Signals {
public: // functions.
    Signals() : m_abrt(0), m_fpe(0), m_ill(0), 
        m_int(0), m_segv(0), m_term(0), m_base(0) {}
    ~Signals() {}

    // const access functions.
    const int sigabrt() const { return m_abrt; }
    const int sigfpe() const { return m_fpe; }
    const int sigill() const { return m_ill; }
    const int sigint() const { return m_int; }
    const int sigsegv() const { return m_segv; }
    const int sigterm() const { return m_term; }
    const int sigbase() const { return m_base; }

    // non-const access functions.
    int sigabrt() { return m_abrt; }
    int sigfpe() { return m_fpe; }
    int sigill() { return m_ill; }
    int sigint() { return m_int; }
    int sigsegv() { return m_segv; }
    int sigterm() { return m_term; }
    int sigbase() { return m_base; }

    // set functions.
    void sigabrt(const int val) { m_abrt = val; }
    void sigfpe(const int val) { m_fpe = val; }
    void sigill(const int val) { m_ill = val; }
    void sigint(const int val) { m_int = val; }
    void sigsegv(const int val) { m_segv = val; }
    void sigterm(const int val) { m_term = val; }
    void sigbase(const int val) { m_base = val; }

private: // data.
    unsigned m_abrt:1;
    unsigned m_fpe:1;
    unsigned m_ill:1;
    unsigned m_int:1;
    unsigned m_segv:1;
    unsigned m_term:1;
    unsigned m_base:1;
};

//////////////////////////////////////////////////////////////////////
// class to keep the state of the signal in the program.
// The recovery variable keeps track of whether the recovery has 
// been performed.
/////////////////////////////////////////////////////////////////////
class SignalState {
public: // functions.
    SignalState() : m_validBuffer(0), m_recovery(0), 
        m_jmpReturn(0), m_key(cBadVal) {}
    ~SignalState() {}

    // const data access functions.
    volatile const int key() const { return m_key; }
    volatile const int validBuffer() const { 
        return m_validBuffer; }
    volatile const int recovery() const { return m_recovery; }
    volatile const int jmpReturn() const { return m_jmpReturn; }
    const Signals& trippedSignals() const { 
        return m_trippedSignals; }
    const jmp_buf& jmpBuffer() const { return m_jmpBuffer; }
    const char* filename() const { return m_filename; }
    volatile const long lineno() const { return m_lineno; }

    // non-const data access functions.
    void key(const int key) { m_key = key; }
    volatile int validBuffer() { return m_validBuffer; }
    volatile int recovery() { return m_recovery; }
    volatile int jmpReturn() { return m_jmpReturn; }
    Signals& trippedSignals() { return m_trippedSignals; }
    jmp_buf& jmpBuffer() { return m_jmpBuffer; }
    char* filename() { return m_filename; }
    volatile long lineno() { return m_lineno; }

    // Data set functions.
    void validBuffer(const int val) { m_validBuffer = val; }
    void recovery(const int val) { m_recovery = val; }
    void jmpReturn(const int val) { m_jmpReturn = val; 
        // Based on bad jmp return value, recovery is disabled.
        if (val == cBadVal ) recovery(cBadVal); }
    void lineno(const long line) { m_lineno = line; }
    void saveLocation(const char* file, const long line) {
        std::strcpy((char*)filename(), file);
        lineno(line);
    }

private: // data.
    volatile int    m_key;
    volatile int    m_validBuffer;
    volatile int    m_recovery;
    volatile int    m_jmpReturn;
    Signals         m_trippedSignals;
    jmp_buf         m_jmpBuffer;
    char            m_filename[cMaxFile];
    volatile long   m_lineno;
};

/////////////////////////////////////////////////////////////////////
// Class to implement the signal handling functionality. 
// It is basically a helper class that provides high level 
// functionality to signal handling.
/////////////////////////////////////////////////////////////////////
class SignalHandler {
public: // functions.
    SignalHandler() {}
    ~SignalHandler() {}

public: // access functions.
    static int currKey() { return m_currKey; }
    static int prevKey() { return m_prevKey; }
    static int currIndex() { return m_currIndex; }
    static int prevIndex() { return m_prevIndex; }
    static int stackPointer() {return m_stackPointer; }
    static Signals& uninitializedSignals() { 
        return m_uninitializedSignals; }
    static SignalState& signalState() { 
        return m_signalState[currIndex()]; }
    static bool forceExit() { return m_forceExit; }
    static bool inHandler() { return m_inHandler; }
    static std::mutex& mutex() { return m_mutex; }

    // Data set functions.
    static void currKey(const int val) {
        std::scoped_lock sl(mutex()); m_currKey = val; }
    static void prevKey(const int val) {
        std::scoped_lock sl(mutex()); m_prevKey = val; }
    static void currIndex(const int val) { 
        std::scoped_lock sl(mutex()); m_currIndex = val; }
    static void prevIndex(const int val) { 
        std::scoped_lock sl(mutex()); m_prevIndex = val; }
    static void forceExit(const bool val) { 
        std::scoped_lock sl(mutex()); m_forceExit = val; }
    static void stackPointer(const int val) {
        std::scoped_lock sl(mutex()); m_stackPointer = val; }
    static void inHandler(const bool val) {
        std::scoped_lock sl(mutex()); m_inHandler = val; }

public: // utility functions.
    // data access functions.
    static int validBuffer() { 
        return signalState().validBuffer(); }
    static int recovery() { 
        return signalState().recovery(); }
    static Signals& trippedSignals() { 
        return signalState().trippedSignals(); }
    static int jmpReturn() { 
        return signalState().jmpReturn(); }
    static jmp_buf& jmpBuffer() { 
        return signalState().jmpBuffer(); }
    static char* filename() { 
        return signalState().filename(); }
    static long lineno() { 
        return signalState().lineno(); }

    // Data set functions.
    static int validBuffer(const int val) { 
        std::scoped_lock sl(mutex()); signalState().validBuffer(val); 
        return val; }
    static int recovery(const int val) { 
        std::scoped_lock sl(mutex()); signalState().recovery(val); 
        return val; }
    static int jmpReturn(const int val) { 
        std::scoped_lock sl(mutex()); signalState().jmpReturn(val); 
        return val; }

    // Utility functions.
    static int findSignalState(const int key);
    static int bindKeyToState(const int key);
    static int presetKeyAndIndex(const int key);
    static int saveKeyAndIndex();
    static int revertKeyAndIndex();
    static int createKey();
    static void incrStackPointer() {
        std::scoped_lock sl(mutex()); ++m_stackPointer; }
    static int saveLocation(const char* file, const long line) {
        std::scoped_lock s1(mutex()); signalState().saveLocation(file, line);
        return 1;
    }

    // Set the signal handler functions.
    static const int setHandler();
    static const int resetHandler();

    // Printing of the handler states.
    static void postUninitializedSignals();
    static void postTrippedSignals();
    static void postStdError();
    static int postWndError();
    static int postWndAbort();
    static int postWndDisabled();
    static int postWndBaseTrap();
    static void postLocation();
    static void postError();
    static void postDisabled();
    static void postAbort();

    // signal handlers.
    static void catchSIGABRT (int sig);
    static void catchSIGFPE (int sig);
    static void catchSIGILL (int sig);
    static void catchSIGINT (int sig);
    static void catchSIGSEGV (int sig);
    static void catchSIGTERM (int sig);
    static void defaultHandler (int sig);
    static void disaster (int sig);

private: // data
    static int              m_currIndex;
    static int              m_prevIndex;
    static int              m_currKey;
    static int              m_prevKey;
    static int              m_stackPointer;
    static bool             m_forceExit;
    static bool             m_inHandler;
    static Signals          m_uninitializedSignals;
    static SignalState      m_signalState[cMaxSignals+1];
    static std::mutex       m_mutex;
};

/////////////////////////////////////////////////////////////////////
#define GENERATESHKEY(var, key) static int var = key
#define CREATESHKEY(var) GENERATESHKEY(var, utl::SignalHandler::createKey())
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define INITSHCORE()                                            \
    utl::SignalHandler::setHandler()
// The function SignalHandler::setHandler() is being called as the 
// signal handlers revert to default handlers when a signal is 
// raised. This macro is highly dependant on the evaluation order.
#define TRYSHCORE(var, file, line)                              \
    CREATESHKEY(var);                                           \
    utl::SignalHandler::saveKeyAndIndex();                      \
    utl::SignalHandler::presetKeyAndIndex(var);                 \
    if (utl::SignalHandler::recovery() != 0) {                  \
        utl::SignalHandler::postWndDisabled();                  \
    }                                                           \
    if (utl::SignalHandler::recovery() == 0 &&                  \
        utl::SignalHandler::validBuffer(1) == 1 &&              \
        utl::SignalHandler::saveLocation(file, line) == 1 &&    \
        utl::SignalHandler::jmpReturn(                          \
            setjmp(utl::SignalHandler::jmpBuffer())             \
        ) == 0)
// The set handler function is involed here, but only if there was 
// a signal raised. This is done because windows seems to insert 
// default handler after the user supplied handler is executed. 
// Set handler  function is used to set the handler back to ours.
#define CATCHSHCORE(var)                                        \
    utl::SignalHandler::presetKeyAndIndex(var);                 \
    utl::SignalHandler::validBuffer(0);                         \
    if (utl::SignalHandler::recovery() != 0 &&                  \
        utl::SignalHandler::setHandler() != utl::cBadVal &&     \
        utl::SignalHandler::revertKeyAndIndex())
#define POSTABORTMSGCORE() utl::SignalHandler::postWndAbort()
#define POSTDISABLEDMSGCORE() utl::SignalHandler::postWndDisabled()
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define INITSHTEST()            INITSHCORE()
#define TRYSHTEST(var)          TRYSHCORE(var, __FILE__, __LINE__)
#define CATCHSHTEST(var)        CATCHSHCORE(var)
#define POSTABORTMSGTEST()      POSTABORTMSGCORE()
#define POSTDISABLEDMSGTEST()   POSTDISABLEDMSGCORE()
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#if ENABLESH
#define INITSH()                INITSHCORE()
#define TRYSH(var)              TRYSHCORE(var, __FILE__, __LINE__)
#define CATCHSH(var)            CATCHSHCORE(var)
#define POSTABORTMSG()          POSTABORTMSGCORE()
#define POSTDISABLEDMSG()       POSTDISABLEDMSGCORE()
#else // ENABLESH
#define INITSH()                {}
#define TRYSH(var)              if (1)
#define CATCHSH(var)            if (0)
#define POSTABORTMSG()          {}
#define POSTDISABLEDMSG()       {}
#endif // ENABLESH
/////////////////////////////////////////////////////////////////////

}

#endif // __SIGHAND_H__

