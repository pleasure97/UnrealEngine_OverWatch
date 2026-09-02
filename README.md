# 오버워치 모작 멀티플레이어 FPS (Unreal Engine 5 / C++)

Gameplay Ability System과 데디케이티드 서버를 기반으로 개발했고, 게임 사이클부터 게임 콘텐츠, 최적화까지 고려하며 구현했습니다.

| | |
|---|---|
| **개발 기간** | 2024.11 ~ 2026.05 |
| **인원** | 개인 프로젝트 |
| **엔진 / 언어** | Unreal Engine 5.7 / C++ |
| **플레이 영상** | [영상 링크](https://) |

---

> ## 저작권 및 사용 범위 고지 / Copyright & Usage Notice
>
> **본 저장소는 직접 작성한 C++ 소스코드만을 포함합니다.** 모델, 텍스처, 사운드 등 모든 시각 리소스들은
> 저작권 문제로 저장소에서 제외되었습니다.
> 본 저장소의 코드는 **오직 교육용 및 비상업적 용도로만** 사용되어야 합니다.
>
> **This repository contains only C++ source code written by the author.** All visual assets
> (models, textures, audio, etc.) have been excluded due to copyright. 
> The code in this repository is
> **strictly for educational and non-commercial use only.**

## 목차

- [기술 스택](#기술-스택)
- [성능 최적화](#성능-최적화)
- [참고 자료](#참고-자료)

---

## 기술 스택

### 게임플레이

![GAS](https://img.shields.io/badge/Gameplay_Ability_System-1C4ACC?style=flat-square)
![Ability Batching](https://img.shields.io/badge/Ability_Batching-1C4ACC?style=flat-square)
![Ability Stacking](https://img.shields.io/badge/Ability_Stacking-1C4ACC?style=flat-square)
![Ability Recharging](https://img.shields.io/badge/Ability_Recharging-1C4ACC?style=flat-square)

### 네트워크

![Dedicated Server](https://img.shields.io/badge/Dedicated_Server-2D6A9F?style=flat-square)
![Replication Graph](https://img.shields.io/badge/Replication_Graph-2D6A9F?style=flat-square)
![Fast Shared Path](https://img.shields.io/badge/Fast_Shared_Path-2D6A9F?style=flat-square)

### UI

![CommonUI](https://img.shields.io/badge/CommonUI-4A7C59?style=flat-square)
![UMG](https://img.shields.io/badge/UMG-4A7C59?style=flat-square)
![MVC](https://img.shields.io/badge/MVC-4A7C59?style=flat-square)
![Gameplay Message Subsystem](https://img.shields.io/badge/Gameplay_Message_Subsystem-4A7C59?style=flat-square)

### 최적화

![PSO Precaching](https://img.shields.io/badge/PSO_Precaching-B85042?style=flat-square)
![Bundled PSO](https://img.shields.io/badge/Bundled_PSO-B85042?style=flat-square)
![Level Streaming](https://img.shields.io/badge/Level_Streaming-B85042?style=flat-square)
![Packed Level Actor](https://img.shields.io/badge/Packed_Level_Actor-B85042?style=flat-square)
![Actor Pool](https://img.shields.io/badge/Actor_Pool-B85042?style=flat-square)
![Garbage Collection](https://img.shields.io/badge/Garbage_Collection-B85042?style=flat-square)
![Low-Level Memory](https://img.shields.io/badge/Low--Level_Memory-B85042?style=flat-square)

### 분석 도구

![Unreal Insights](https://img.shields.io/badge/Unreal_Insights-36454F?style=flat-square)
![Stat Commands](https://img.shields.io/badge/Stat_Commands-36454F?style=flat-square)

---

## 성능 최적화

모든 수치는 Unreal Insights 측정 기준입니다.

| 영역 | 조치 | Before | After |
|---|---|---|---|
| 레벨 전환 | Seamless Travel 적용 | 3.8s | **42.9ms** |
| 오브젝트 관리 | 2,264개 Static Mesh Actor를 11개 Packed Level Actor로 그룹화 | 44.3ms | **325.8μs** |
| 컴포넌트 등록 | PSO · 애니메이션 · 스폰 · 메모리 4단계로 원인 분리 후 순차 개선 | 118.5ms | **24.4ms** |
| 액터 스폰 | 영웅 액터 풀 구축 + 프레임 예산 여유 페이즈로 스폰 분산 | 967.5ms | **37.6ms** |
| 렌더링 | PSO Precaching 활성화 | 382.7ms | **36ms** |
| 렌더링 | 플레이 테스트로 수집한 Bundled PSO 구축 | 75.9ms | **87.7μs** |
| 메모리 | Low-Level Memory 트레이스 기반 메시 · 텍스처 · 콜리전 최적화 | — | **총 580MB 절감** |
| 가비지 컬렉션 | 영구 상주 오브젝트를 Root Set에서 제외, 전략적 호출 시점 설계 | 35,546개 | **7,828개** |
| 네트워크 | Cond_OwnerOnly / REPNOTIFY_OnChanged 조정, 불필요 RPC 제거 | — | **패킷당 약 150Byte 절감** |
| 네트워크 | Replication Graph에 Fast Shared Path 구성 | 연결당 10회 | **1회 직렬화** |

---

## 참고 자료

- Stephen Ulibarri, *Gameplay Ability System* / *Dedicated Server* (Udemy) — GAS 및 서버 구조 학습
- [GASDocumentation](https://github.com/tranek/GASDocumentation)
- Unreal Engine 공식 문서 — UI 최적화 가이드라인