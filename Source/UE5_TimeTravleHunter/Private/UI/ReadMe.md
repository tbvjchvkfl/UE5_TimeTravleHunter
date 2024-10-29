Contents
-

### 1. Inventory System


Inventory System
-

- ### Inventory Component
  > - 자료구조 Map을 사용해서 인벤토리 내 위치와 그 위치에 따른 값을 가지도록 구현했습니다. 
  > - 인벤토리가 가지고 있어야 하는 Size와 인벤토리의 가로 크기 Width를 지정하였고, 현재 인벤토리의 공간이 비어있는지 채워져있는지를 판단하기 위해 TMap<FVector2D, bool> InventoryState를 만들어주었습니다.
  > - 게임 내에서 재화를 습득했을 경우 인벤토리가 단순 텍스트로 별도 표시해줄 생각이었으므로 CoinInventory라는 변수를 만들어 주었고, 실제 아이템에 대한 정보를 담아둘 TMap<FVector2D, APickUpItem*> ItemInventory를 만들어주었습니다.
  <pre>
   <code>
      UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      int32 InventorySize;
      
      UPROPERTY(EditAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      int32 InventoryWidth;
      
      UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      TMap<FVector2D, APickUpItem *> ItemInventory;
      
      UPROPERTY(VisibleAnywhere, Category = "Inventory | Property", meta = (AllowPrivateAccess = "true"))
      TMap<FVector2D, bool> InventoryState;
      
      UPROPERTY()
      int32 CoinInventory;
   </code>
  </pre>

    > - 캐릭터가 APickUpItem과 인터렉션 하면 CheckItem 함수를 통해 Item의 타입을 확인하여 Coin이면 CoinInventory에 값이 추가되고 그 외 타입이면 AddItem함수가 호출되도록 구현해주었습니다.
    > - 멀티 캐스트 델리게이트를 바인딩하여 아이템을 획득할 때마다 인벤토리 내용이 업데이트 될 수 있도록 구현했습니다.
    <pre>
   <code>
      switch (Item->GetItemType())
      {
      	case EItemType::Coin:
      	{
      		CoinInventory += Item->GetCurrentQuantity();
      	}
      	break;
      	case EItemType::Weapon:
      	{
      		AddItem(Item);
      	}
      	break;
      	case EItemType::Consumable:
      	{
      		AddItem(Item);
      	}
      	break;
      	default:
      	{
      		return;
      	}
      	break;
      }
      OnInventoryUpdate.Broadcast();
   </code>
  </pre>
  
    > - InventoryComponent::BeginPlay()함수에 초기화함수를 호출해주었습니다.
    > - 처음에는 FVector2D 타입이나, FIntPoint타입의 변수를 사용하여 인벤토리의 크기를 설정해주려하였으나, 그렇게 되면 이중 for루프를 돌게 되어 시간 복잡도가 n^2이 되어 아래 코드와 같은 방식으로 for루프를 한번만 돌도록 구현하였습니다.
  <pre>
   <code>
      void UInventoryComponent::InitializeInventory()
      {
      	ItemInventory.Empty();
      	InventoryState.Empty();
      	if (InventoryState.IsEmpty())
      	{
      		int32 Column = 0;
      		int32 Row = 0;
      		for (int32 i = 0; i < InventorySize; i++)
      		{
      			InventoryState.Add(FVector2D(Column, Row), false);
      			if (Column == InventoryWidth - 1)
      			{
      				Column = 0; 
      				Row++;
      			}
      			else
      			{
      				Column++;
      			}
      		}
      	}
      }
   </code>
  </pre>