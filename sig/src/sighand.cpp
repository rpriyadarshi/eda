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

#include <iostream>
#include <float.h>
#include <assert.h>
#include <bitset>
#include <math.h>
#include <signal.h>
#include <setjmp.h>
#include <mutex>
#include <cstring>

#include "common.h"
#include "sighand.h"

namespace utl {
std::mutex SignalHandler::m_mutex;

//////////////////////////////////////////////////////////////////////
// Mandatory static variable definition.
//////////////////////////////////////////////////////////////////////
Signals SignalHandler::m_uninitializedSignals;
SignalState SignalHandler::m_signalState[];
int SignalHandler::m_currIndex(0);
int SignalHandler::m_prevIndex(0);
int SignalHandler::m_currKey = cBadVal;
int SignalHandler::m_prevKey = cBadVal;
bool SignalHandler::m_forceExit(false);
bool SignalHandler::m_inHandler(false);
int SignalHandler::m_stackPointer(0);

const int SignalHandler::resetHandler() {
    std::scoped_lock sl(mutex());
    int success = 0;
    // Removing signal handlers.
    if (signal(SIGABRT, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    if (signal(SIGFPE, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    if (signal(SIGILL, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    if (signal(SIGSEGV, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    if (signal(SIGTERM, SIG_DFL) == SIG_ERR) {
        success = cBadVal;
    }
    
    return success;
}

const int SignalHandler::setHandler() {
    std::scoped_lock sl(mutex());

    int success = 0;
    // Adding signal handlers.
    if (signal(SIGABRT, catchSIGABRT) == SIG_ERR) {
        uninitializedSignals().sigabrt(1);
        success = cBadVal;
    }
    if (signal(SIGFPE, catchSIGFPE) == SIG_ERR) {
        uninitializedSignals().sigfpe(1);
        success = cBadVal;
    }
    if (signal(SIGILL, catchSIGILL) == SIG_ERR) {
        uninitializedSignals().sigill(1);
        success = cBadVal;
    }
    if (signal(SIGINT, catchSIGINT) == SIG_ERR) {
        uninitializedSignals().sigint(1);
        success = cBadVal;
    }
    if (signal(SIGSEGV, catchSIGSEGV) == SIG_ERR) {
        uninitializedSignals().sigsegv(1);
        success = cBadVal;
    }
    if (signal(SIGTERM, catchSIGTERM) == SIG_ERR) {
        uninitializedSignals().sigterm(1);
        success = cBadVal;
    }
    
    return success;
}

void SignalHandler::catchSIGABRT (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigabrt()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigabrt(1);
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

void SignalHandler::catchSIGFPE (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigfpe()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigfpe(1);
#if FPU_RESET
    _fpreset(); // reset the floating point package.
#endif
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

void SignalHandler::catchSIGILL (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigill()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigill(1);
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

void SignalHandler::catchSIGINT (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigint()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigint(1);
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

void SignalHandler::catchSIGSEGV (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigsegv()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigsegv(1);
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

void SignalHandler::catchSIGTERM (int sig) {
    if (inHandler()) { // already in signal handler!
        disaster(sig);
    }
    inHandler(true);
    bindKeyToState(currKey());
    incrStackPointer();
    if (! validBuffer() || 
        forceExit() || 
        stackPointer() > cMaxSignals || 
        trippedSignals().sigterm()) {
        defaultHandler(sig); // Revert to the base signal handler.
        inHandler(false);
        
        return;
    }

    trippedSignals().sigterm(1);
    validBuffer(0);
    postWndError();
    inHandler(false);
    

    longjmp(signalState().jmpBuffer(), cBadVal);
}

// performing linear search since too many signal are fatal anyway!
int SignalHandler::findSignalState(const int key) {
    std::scoped_lock sl(mutex());

    if (key < 0) return cBadVal;
    for (int i = 0; i <= stackPointer(); i++) {
        SignalState& state = m_signalState[i];
        if (state.key() == key) {
            return i;
        }
    }

    
    return cBadVal;
}

// Setup the current signal index. This is done be first finding if 
// there is signal state already set up corresponding to the key.
// If the key is already present, set the current index based on the
// key. Otherwise choose the current stack pointer and add the key.
int SignalHandler::bindKeyToState(const int key) {
    int sigIndex = findSignalState(key);
    if (sigIndex != cBadVal) {
        currIndex(sigIndex);
    }
    else {
        currIndex(stackPointer());
        {
            std::scoped_lock sl(mutex());
            signalState().key(key); 
        }
    }

    return 1;
}

// Setup the current signal key. This is done be first finding if 
// there is signal state already set up corresponding to the key.
// If the key is already present, set the current index based on the
// key. Otherwise choose the current stack pointer.
int SignalHandler::presetKeyAndIndex(const int key) {
    // The key is set as current key to enable accessing the keyed
    // signal state while in signal handler function as there is no
    // way of passing key as an argument to the handler.
    currKey(key); 
    int sigIndex = findSignalState(key);
    if (sigIndex != cBadVal) {
        currIndex(sigIndex);
    }
    else {
        currIndex(stackPointer());
    }

    
    return 1;
}

// Previous key is saved to enable reverting back is the signal has 
// been received for the current key before. Not saving it will keep 
// the current key, with the corresponding jump buffer invalidated 
// after signal. Thiswill lead to default signal handler being 
// activated and the program will exit.
int SignalHandler::saveKeyAndIndex() {
    prevKey(currKey());
    prevIndex(currIndex());

    return 1;
}

// Revert the previous key if the current key is disabled due to signal.
int SignalHandler::revertKeyAndIndex() {
    currKey(prevKey());
    currIndex(prevIndex());

    return 1;
}

// The key is simply a running static integer.
int SignalHandler::createKey() {
    std::scoped_lock sl(mutex());

    static int key = 0;
    key++;
 
    return key;
}

// The default handler just posts error message and exits.
void SignalHandler::defaultHandler(int signal) {
    postStdError();

    assert(0); // Help to debuggers.
    exit(2);
}

// The default handler just posts error message and exits.
void SignalHandler::disaster(int signal) {
    std::scoped_lock sl(mutex());
    
    if (trippedSignals().sigabrt()) 
        std::cerr << "-- ERROR: Caught SIGABRT!!!" << std::endl;
    if (trippedSignals().sigfpe()) 
        std::cerr << "-- ERROR: Caught SIGFPE!!!" << std::endl;
    if (trippedSignals().sigill()) 
        std::cerr << "-- ERROR: Caught SIGILL!!!" << std::endl;
    if (trippedSignals().sigint()) 
        std::cerr << "-- ERROR: Caught SIGINT!!!" << std::endl;
    if (trippedSignals().sigsegv()) 
        std::cerr << "-- ERROR: Caught SIGSEGV!!!" << std::endl;
    if (trippedSignals().sigterm()) 
        std::cerr << "-- ERROR: Caught SIGTERM!!!" << std::endl;

    
    assert(0); // Help to debuggers.
    exit(3);
}

void SignalHandler::postStdError() {
    postError();
}

int SignalHandler::postWndError() {
    postError();
    
    return true;
}

int SignalHandler::postWndAbort() {
    postAbort();
    
    return true;
}

int SignalHandler::postWndDisabled() {
    postDisabled();
    postLocation();

    return true;
}

int SignalHandler::postWndBaseTrap() {
    postError();
    
    return true;
}

void SignalHandler::postError() {
    postTrippedSignals();
}

void SignalHandler::postDisabled() {
    std::scoped_lock sl(mutex());
    std::cerr << "-- WARNING: TRYSH block is disabled!" << std::endl;
}

void SignalHandler::postAbort() {
    std::scoped_lock sl(mutex());
    std::cerr << "-- ERROR: Aborting execution!" << std::endl;
}

void SignalHandler::postLocation() {
    std::scoped_lock sl(mutex());
    std::cerr << ">> Location: " 
        << SignalHandler::filename() << ":" 
        << SignalHandler::lineno() << std::endl;
}

void SignalHandler::postUninitializedSignals() {
    std::scoped_lock sl(mutex());
    if (uninitializedSignals().sigabrt()) 
        std::cerr << "-- ERROR: Could not set SIGABRT" << std::endl;
    if (uninitializedSignals().sigfpe()) 
        std::cerr << "-- ERROR: Could not set SIGFPE" << std::endl;
    if (uninitializedSignals().sigill()) 
        std::cerr << "-- ERROR: Could not set SIGILL" << std::endl;
    if (uninitializedSignals().sigint()) 
        std::cerr << "-- ERROR: Could not set SIGINT" << std::endl;
    if (uninitializedSignals().sigsegv()) 
        std::cerr << "-- ERROR: Could not set SIGSEGV" << std::endl;
    if (uninitializedSignals().sigterm()) 
        std::cerr << "-- ERROR: Could not set SIGTERM" << std::endl;
}

void SignalHandler::postTrippedSignals() {
    std::scoped_lock sl(mutex());
    if (trippedSignals().sigabrt()) 
        std::cerr << "-- ERROR: Caught SIGABRT!!!" << std::endl;
    if (trippedSignals().sigfpe()) 
        std::cerr << "-- ERROR: Caught SIGFPE!!!" << std::endl;
    if (trippedSignals().sigill()) 
        std::cerr << "-- ERROR: Caught SIGILL!!!" << std::endl;
    if (trippedSignals().sigint()) 
        std::cerr << "-- ERROR: Caught SIGINT!!!" << std::endl;
    if (trippedSignals().sigsegv()) 
        std::cerr << "-- ERROR: Caught SIGSEGV!!!" << std::endl;
    if (trippedSignals().sigterm()) 
        std::cerr << "-- ERROR: Caught SIGTERM!!!" << std::endl;
}

}

