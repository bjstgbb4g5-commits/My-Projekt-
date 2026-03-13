#include "NoxFragmentSubsystem.h"
#include "NavigationSystem.h"
#include "Engine/World.h"

void UNoxFragmentSubsystem::GenerateFragments(int32 Count)
{
    UWorld* World = GetWorld();
    if (!World || !World->IsServer()) return;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
    if (!NavSys) return;

    for (int32 i = 0; i < Count; ++i)
    {
        FNavLocation NavLoc;
        bool bFound = NavSys->GetRandomPointInNavigableRadius(FVector::ZeroVector, 80000.f, NavLoc);
        if (!bFound) continue;

        FNoxFragment Fragment;
        Fragment.FragmentID = NextFragmentID++;
        Fragment.Location = NavLoc.Location;
        Fragment.bDiscovered = false;
        Fragments.Add(Fragment.FragmentID, Fragment);
    }
}

void UNoxFragmentSubsystem::DiscoverFragment(int32 FragmentID)
{
    FNoxFragment* Fragment = Fragments.Find(FragmentID);
    if (Fragment)
    {
        Fragment->bDiscovered = true;
    }
}
