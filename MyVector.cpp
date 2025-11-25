#pragma once
#include "MyVector.h"
#include <stdexcept> // std::out_of_range 예외 처리를 위해 필요

// MyVector 클래스 멤버 함수 구현

// push_back: 배열이 꽉 차면 2배로 확장 후 추가
template<typename T>
void MyVector<T>::push_back(const T& val) {
    if (length == cap) {
        // 용량이 0이면 1로, 아니면 2배로 확장
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        T* new_data = new T[new_cap];

        // 기존 데이터 복사
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = data[i];
        }

        // 기존 메모리 해제 및 포인터 교체
        if (data) delete[] data;
        data = new_data;
        cap = new_cap;
    }
    data[length++] = val;
}

// pop_back: 마지막 요소 제거 (실제 메모리는 유지하고 length만 줄임)
template<typename T>
void MyVector<T>::pop_back() {
    if (length > 0) {
        length--;
    }
}

// size: 현재 요소 개수 반환
template<typename T>
size_t MyVector<T>::size() const {
    return length;
}

// capacity: 현재 할당된 메모리 크기 반환
template<typename T>
size_t MyVector<T>::capacity() const {
    return cap;
}

// empty: 비어있는지 확인
template<typename T>
bool MyVector<T>::empty() const {
    return length == 0;
}

// operator[]: 인덱스로 요소 접근 (범위 체크 없음, 속도 빠름)
template<typename T>
T& MyVector<T>::operator[](int i) {
    return data[i];
}

// at: 인덱스로 요소 접근 (범위 체크 있음, 안전함)
template<typename T>
T& MyVector<T>::at(size_t i) {
    if (i >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[i];
}

// front: 첫 번째 요소 반환
template<typename T>
T& MyVector<T>::front() {
    return data[0];
}

// back: 마지막 요소 반환
template<typename T>
T& MyVector<T>::back() {
    return data[length - 1];
}

// clear: 모든 요소 제거 (메모리는 해제하지 않고 length만 0으로)
template<typename T>
void MyVector<T>::clear() {
    length = 0;
}

// // insert: 지정 위치에 요소 삽입
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::insert(Iterator pos, const T& value) {
    // 현재 iterator의 위치(인덱스) 계산
    int index = pos - begin();

    // 공간 부족 시 확장 (push_back과 동일한 로직)
    if (length == cap) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < length; ++i) {
            new_data[i] = data[i];
        }
        if (data) delete[] data;
        data = new_data;
        cap = new_cap;
    }

    // 삽입 위치 뒤의 요소들을 한 칸씩 뒤로 밀기
    for (size_t i = length; i > (size_t)index; --i) {
        data[i] = data[i - 1];
    }

    // 값 삽입
    data[index] = value;
    length++;

    return Iterator(data + index);
}

// erase: 지정 위치 요소 제거
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::erase(Iterator pos) {
    int index = pos - begin();

    // 삭제 위치 뒤의 요소들을 한 칸씩 앞으로 당기기
    for (size_t i = index; i < length - 1; ++i) {
        data[i] = data[i + 1];
    }

    length--;
    return Iterator(data + index);
}

// begin: 첫 번째 요소를 가리키는 반복자 반환
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::begin() {
    return Iterator(data);
}

// end: 마지막 요소 다음을 가리키는 반복자 반환
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::end() {
    return Iterator(data + length);
}


// 비교 연산자 구현


template<typename T>
bool MyVector<T>::operator==(const MyVector& other) const {
    if (length != other.length) return false;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

template<typename T>
bool MyVector<T>::operator!=(const MyVector& other) const {
    return !(*this == other);
}

template<typename T>
bool MyVector<T>::operator<(const MyVector& other) const {
    // 사전순 비교 (Lexicographical comparison)
    size_t min_len = (length < other.length) ? length : other.length;
    for (size_t i = 0; i < min_len; ++i) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return length < other.length;
}

template<typename T>
bool MyVector<T>::operator>(const MyVector& other) const {
    return other < *this;
}

template<typename T>
bool MyVector<T>::operator<=(const MyVector& other) const {
    return !(*this > other);
}

template<typename T>
bool MyVector<T>::operator>=(const MyVector& other) const {
    return !(*this < other);
}


// Iterator 내부 클래스 구현


// 역참조 (*it)
template<typename T>
T& MyVector<T>::Iterator::operator*() {
    return *ptr;
}

// 전위 증가 (++it)
template<typename T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator++() {
    ++ptr;
    return *this;
}

// 전위 감소 (--it)
template<typename T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator--() {
    --ptr;
    return *this;
}

// 산술 연산 (+ n)
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::Iterator::operator+(int n) const {
    return Iterator(ptr + n);
}

// 산술 연산 (- n)
template<typename T>
typename MyVector<T>::Iterator MyVector<T>::Iterator::operator-(int n) const {
    return Iterator(ptr - n);
}

// 비교 (==)
template<typename T>
bool MyVector<T>::Iterator::operator==(const Iterator& other) const {
    return ptr == other.ptr;
}

// 비교 (!=)
template<typename T>
bool MyVector<T>::Iterator::operator!=(const Iterator& other) const {
    return ptr != other.ptr;
}

// Iterator 간 뺄셈 (거리 계산)
template<typename T>
int MyVector<T>::Iterator::operator-(const Iterator& other) const {
    return static_cast<int>(ptr - other.ptr);
}